#include <agents/preys/planners/point_planner.h>

using namespace std;
using namespace cell_world;

Point_planner::Point_planner(World &world, const Cell_group &gates, const Cell &start, const Cell &goal, Planning_strategy ps, Planning_unit pu, unsigned int i, Reward_config rc, unsigned int k, cell_world::Paths &p, unsigned int iterations) :
        Planner( world, start, goal,pu, i, rc, k,p, iterations),
        planning_strategy(ps),
        _habit_set(world, gates),
        _last_destination(Not_found)
{
}

void Point_planner::update_state(unsigned int &) {
    options = _habit_set[cell()];
    rewards = vector<double>(options.size(),0);
    visits = vector<unsigned int>(options.size(),0);
}

void Point_planner::plan() {
    Model &model = set.get_valid_model();
    unsigned int option = Chance::dice(options.size());
    auto prey_cell = set.prey.cell();
    auto move = options[option].get().policy(prey_cell);
    set.prey.set_move(move);
    auto habits = options;
    unsigned int current_habit = option;
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
    unsigned int option = Chance::pick_best(1,rewards);
    set_value(rewards[option]);
    _last_destination = options[option].get().destination.id;
    _world[_last_destination].icon = Icon::Custom_icon_1;
    return options[option].get().policy(cell());
}
