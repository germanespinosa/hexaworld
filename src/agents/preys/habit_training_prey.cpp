#include <agents/preys/habit_training_prey.h>
#include <fstream>
#include <iostream>
#include <utility>

using namespace cell_world;
using namespace std;

Habit_training_prey::Habit_training_prey(std::vector<Habit> &habits, const Reward_config rewards, double best_action_probability)
: _habits ( habits )
, _rewards ( rewards )
, episodes(0)
, successes(0)
, _current_habit(0)
, _sizes(_habits.size())
{
    for (uint32_t i=0;i<_sizes.size();i++) {
        _sizes[i] = _habits[i].size();
    }
}

const Cell & Habit_training_prey::start(uint32_t t) {
    episodes++;
    //pick the next habit
    _current_habit = cell_world::Chance::pick(_sizes); //pick a random habit with probability direct to habit size;
    auto &habit = _habits[_current_habit];
    // set the goal to the habit destination
    set_goal(habit.destination);
    //clear the history
    _history.clear();
    //determine the start cell with probability inverse to previous visits
    vector<uint32_t> visits(_sizes[_current_habit]);
    for (uint32_t i=0;i<visits.size();i++){
        visits[i] = habit.values[i].visits;
    }
    _start_cell_index = Chance::pick_inverse(visits);
    //determine the start action
    uint32_t action_count = habit.values[_start_cell_index].actions.size();
    vector<uint32_t > action_visits(action_count);
    for (uint32_t i = 0 ; i < action_count ; i++) action_visits[i] = habit.values[_start_cell_index].actions[i].visits;
    _start_action_index = Chance::pick_inverse(action_visits);
    _next_move = habit.values[_start_cell_index].actions[_start_action_index].move;
    return habit.nodes[_start_cell_index]; //pick a random start cell
}

void Habit_training_prey::update(const State &state) {
    set_status(Action_ready);
    if (state.iteration == 0){
        return;
    }
    auto &habit = _habits[_current_habit];
    int32_t cell_index = habit.nodes.find(cell());
    if (cell_index == Not_found) {
        _result = Fail;
        set_status(Finished);
    } else {
        _next_move = habit.values[cell_index].policy();
    }
}

Coordinates Habit_training_prey::get_move() {
    return _next_move;
}

void Habit_training_prey::end(Episode_result r, uint32_t l, const cell_world::History &) {
    if (r == Success) successes++;
    auto &habit = _habits[_current_habit];
    habit.add_reward(_start_cell_index,_start_action_index, _rewards, r, l);
    habit.end_episode( _start_cell_index, r);
}
