#include <hexaworld.h>
#include <habit_training.h>
#include <fstream>
#include <iostream>


using namespace cell_world;
using namespace std;

Habit_training::Habit_training(std::vector<Habit> &habits, const Reward_config rewards, Action_set actions, cell_world::Probabilities & probabilities)
: _habits ( habits )
, _rewards ( rewards )
, _probabilities ( probabilities )
, _action_set(actions)
, Agent({"Prey",2})
{
    _current_habit = 0;
    vector<uint32_t > chances;
    for (auto &habit:habits){
        chances.push_back(habit.cells.size());
    }
    _habit_probability = Probabilities(chances);
}

const Cell & Habit_training::start_episode(const cell_world::State &state) {
    set_status(Active);
    _iteration = 0;
    _episode_reward = 0;
    _predator_index = state.find("Predator");
    while (_habits[_current_habit = _habit_probability.pick()].convergence); //pick a random habit to train
    auto &habit = _habits[_current_habit];
    auto &cell =_habits[_current_habit].cells[Probabilities::dice(habit.cells.size())]; //pick a random start cell
    return cell;
}


void Habit_training::update_state(const cell_world::State &state) {
    _iteration = state.iteration;
    auto predator_cell = state.agents_data[_predator_index].cell;
    auto prey_cell = cell();
    if (prey_cell == _habits[_current_habit].destination){
        set_status(Agent_status::Finished); // success
        _episode_reward = _rewards.success_reward;
    } else if ( _habits[_current_habit].gate_cells.contains(prey_cell)){
        set_status(Agent_status::Finished); // wrong gate -- fail
        _episode_reward = _rewards.failure_reward;
    } else if (predator_cell.id == prey_cell.id) {
        set_status(Agent_status::Finished); //eaten -- fail
        _episode_reward = _rewards.failure_reward;
    } else if ( _habits[_current_habit].cells.find(prey_cell)== Not_found ) {
        set_status(Agent_status::Finished); // wrong sub world
        _episode_reward = _rewards.failure_reward;
    }
}

Agent_action &Habit_training::get_action() {
    auto habit = _habits[_current_habit];
    int32_t cell_index = habit.cells.find(cell());
    if (cell_index==Not_found) {
    }
    uint32_t action_index = _probabilities.pick(habit.values[cell_index].rewards);
    _add_history(cell_index,action_index);
    return _action_set[action_index];
}

uint32_t Habit_training::action_ready() {
    return _iteration;
}

void Habit_training::end_episode(const cell_world::State &) {
    auto &habit = _habits[_current_habit];
    for (uint32_t i = 0 ; i < _iteration ; i++){
        uint32_t steps_to_end = _iteration - i ;
        double step_reward = _episode_reward * pow(_rewards.discount,steps_to_end) - _rewards.step_cost * steps_to_end;
        auto &values = habit.values[_history[i].cell];
        double old_value = values.rewards[_history[i].action] * values.visits;
        double old_length = values.lenght * values.visits;
        values.visits++;
        double new_reward = (old_value + step_reward) / values.visits;
        values.rewards[_history[i].action] = new_reward;
        values.lenght = (old_length + steps_to_end) / values.visits;
    }
}

void Habit_training::_add_history(uint32_t cell_index, uint32_t action_index) {
    while(_history.size()<=_iteration) _history.emplace_back();
    _history[_iteration].cell = cell_index;
    _history[_iteration].action = action_index;
}
