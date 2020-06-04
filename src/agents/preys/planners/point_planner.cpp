#include <agents/preys/planners/point_planner.h>

using namespace std;
using namespace cell_world;

Point_planner::Point_planner(World &world, const Cell_group &gates, const Cell &start, const Cell &goal, Planning_strategy ps, Planning_unit pu, uint32_t i, Reward_config rc, uint32_t k, cell_world::Paths &p) :
        Planner( world, start, goal,pu, i, rc, k,p),
        planning_strategy(ps),
        _habit_set(world, gates),
        _last_destination(Not_found)
{
}

void Point_planner::update_state(uint32_t &) {
    options = _habit_set[cell()];
    rewards = vector<double>(options.size(),0);
    visits = vector<uint32_t>(options.size(),0);
}

void Point_planner::plan() {
    Model &model = set.get_valid_model();
    uint32_t option = Chance::dice(options.size());
    auto prey_cell = set.prey.cell();
    auto move = options[option].get().policy(prey_cell);
    set.prey.set_move(move);
    auto habits = options;
    uint32_t current_habit = option;
    auto destination = habits[current_habit].get().destination;
    while(model.update()) {
        auto cell = set.prey.cell();
        if ( destination == cell ){ // reach a point of interest... time to check our options
            habits = _habit_set[cell];
            current_habit = Chance::dice(habits.size());
            destination = habits[current_habit].get().destination;
        }
        if (planning_strategy==Planning_strategy::micro_habits)
            move = habits[current_habit].get().policy(cell);
        else
            move = _paths.get_move(cell,destination);
        set.prey.set_move(move);
    }
    model.end_episode();
    double reward = _reward_config.value(set.prey.result, set.prey.lenght);
    rewards[option] = ( rewards[option] * visits[option] + reward )/( visits[option] + 1 );
    visits[option]++;
}

cell_world::Move Point_planner::get_best_move() {
    if (_last_destination != Not_found) _world[_last_destination].icon = Icon::No_icon;
    uint32_t option = Chance::pick_best(1,rewards);
    reward_history.push_back(rewards[option]);
    _last_destination = options[option].get().destination.id;
    _world[_last_destination].icon = Icon::Custom_icon_1;
    return options[option].get().policy(cell());
}
