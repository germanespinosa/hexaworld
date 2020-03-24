#include <hexaworld.h>
#include <habit_training.h>
#include <fstream>
#include <iostream>
#include <utility>

using namespace cell_world;
using namespace std;

Habit_training::Habit_training(std::vector<Habit> &habits, const Reward_config rewards, double best_action_probability)
: _habits ( habits )
, _rewards ( rewards )
, episodes(0)
, _iteration(0)
, _episode_result(Unknown)
, success(0)
, _current_habit(0)
, Agent({"Prey",2})
{}

const Cell & Habit_training::start_episode(uint32_t) {
    L("const Cell & Habit_training::start_episode(const State &state) start");
    episodes++;

    //pick the next habit
    vector<uint32_t> habit_sizes;
    for (auto &h:_habits) habit_sizes.push_back(h.size());
    _current_habit = _habit_probability.pick(habit_sizes); //pick a random habit with probability direct to habit size;
    auto &habit = _habits[_current_habit];

    //set the result to unknown
    _episode_result = Unknown;

    //let the model known the agent is ready
    set_status(Action_ready);

    //clear the history
    _history.clear();

    //determine the start cell with probability inverse to previous visits
    vector<uint32_t > visits;
    for (auto &v:habit.values)visits.push_back(v.visits);
    _start_cell_index = Chance::pick_inverse(visits);
    //determine the start action
    vector<uint32_t > action_visits;
    for (auto &a:habit.values[_start_cell_index].actions)action_visits.push_back(a.visits);
    _start_action_index = Chance::pick_inverse(action_visits);
    _history.push_back({_start_cell_index,_start_action_index});
    _next_move = habit.values[_start_cell_index].actions[_start_action_index].move;

    L("const Cell & Habit_training::start_episode(const State &state) end " << _start_cell_index );
    return habit.nodes[_start_cell_index]; //pick a random start cell
}

void Habit_training::update_state(const State &state) {
    L("Habit_training::update_state(const State &state) start");
    _iteration = state.iteration;
    set_status(Action_ready);
    auto prey_cell = cell();
    if ( !state.agents_data.empty()) {
        auto predator_cell = state.agents_data[0].cell;
        if (predator_cell.location.dist(prey_cell.location)<2) {
            set_status(Agent_status::Finished); //eaten -- fail
            _episode_result = Fail;
            return;
        }
    }
    auto &habit = _habits[_current_habit];

    if (prey_cell== habit.destination){
        set_status(Agent_status::Finished); // success
        _episode_result = Success;
        success++;
        return;
    }
    int32_t cell_index = habit.nodes.find(prey_cell);
    if ( cell_index == Not_found ) {
        set_status(Agent_status::Finished); // wrong sub world only for stochastic simulations
        return;
    }
    _next_move = habit.values[cell_index].policy();
    L("Habit_training::update_state(const State &state) end");
}

Coordinates Habit_training::get_move() {
    L("Coordinates Habit_training::get_move() start");
    return _next_move;
    L("Coordinates Habit_training::get_move() end");
}

void Habit_training::end_episode(const State &) {
    L("void Habit_training::end_episode(const State &) start");
    auto &habit = _habits[_current_habit];
    habit.add_reward(_start_cell_index,_start_action_index, _rewards, _episode_result, _iteration);
    habit.end_episode( _start_cell_index, _episode_result);
    L("void Habit_training::end_episode(const State &) end");
}