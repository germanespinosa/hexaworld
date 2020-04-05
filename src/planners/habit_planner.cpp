#include <habit_planner.h>

using namespace std;
using namespace cell_world;

Habit_planner::Habit_planner(const World &world, const Cell_group &gates, const Cell &start, const Cell &goal, double time, Reward_config rc) :
    Planner( world, start, goal, time, rc),
    _habit_set(world, gates)
{
cout << "here" << endl;
}

void Habit_planner::update_state() {
    options = _habit_set[cell()];
    rewards = vector<double>(options.size(),0);
    visits = vector<uint32_t>(options.size(),0);
    fails = vector<uint32_t>(options.size(),0);
    successes = vector<uint32_t>(options.size(),0);
    unknowns = vector<uint32_t>(options.size(),0);
}

void Habit_planner::plan() {
    Model &model = set.get_valid_model();
    uint32_t option = Chance::dice(options.size());
    auto prey_cell = cell();
    set.prey.set_move(options[option].get().policy(prey_cell));
    set.prey.successes = 0 ;
    set.prey.fails = 0;
    set.prey.unknown = 0;
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
        set.prey.set_move(habits[current_habit].get().policy(cell));
    }
    model.end_episode();
    rewards[option] = ( rewards[option] * visits[option] + set.prey.reward )/( visits[option] + 1 );
    visits[option]++;
    successes[option]+=set.prey.successes;
    fails[option]+=set.prey.fails;
    unknowns[option]+=set.prey.unknown;
    if (set.prey.fails > 1) cout << "error" << endl;
}

cell_world::Move Habit_planner::get_best_move() {
    uint32_t option = 0;
    for (uint32_t i = 0; i < options.size(); i++) if (rewards[i] > rewards[option]) option = i;
    return options[option].get().policy(cell());
}
