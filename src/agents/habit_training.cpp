#include <hexaworld.h>
#include <habit_training.h>
#include <fstream>
#include <iostream>

using namespace cell_world;
using namespace std;

Habit_training::Habit_training(std::vector<Habit> &habits, const Reward_config rewards, Action_set actions, Chance & probabilities, uint32_t thread, uint32_t threads)
: _fixed_start(false)
, _habits ( habits )
, _rewards ( rewards )
, _probabilities ( probabilities )
, _action_set(actions)
, episodes(0)
, success(0)
, Agent({"Prey",2})
{
    L("Habit_training::Habit_training(std::vector<Habit> &,const Reward_config ,Action_set ,Chance & ,uint32_t ,uint32_t ) start");
    _current_habit = 0;
    vector<uint32_t > chances;
    set_color(Blue);
    for (uint32_t i=0; i<habits.size();i++){
        if (i % threads == thread) {
            auto &habit = habits[i];
            chances.push_back(habit.cells.size());
        }else{
            chances.push_back(0);// will be taken care by another thread
        }
    }
    _habit_probability = Chance(chances);
    L("Habit_training::Habit_training(std::vector<Habit> &,const Reward_config ,Action_set ,Chance & ,uint32_t ,uint32_t ) end");
}

const Cell & Habit_training::start_episode(const State &state) {
    L("Habit_training::start_episode(const State &) start");
    episodes++;
    _iteration = 0;
    _current_habit = _habit_probability.pick(); //pick a random habit with probability direct to habit size;
    _episode_result = Unknown;
    auto &habit = _habits[_current_habit];
    auto &cell =_fixed_start ? _start : _habits[_current_habit].cells[Chance::dice(habit.cells.size())]; //pick a random start cell
    set_status(Action_ready);
    L("Habit_training::start_episode(const State &) end");
    return cell;
}


void Habit_training::update_state(const State &state) {
    L("Habit_training::update_state(const State &) start");
    _iteration = state.iteration;
    set_status(Action_ready);
    auto prey_cell = cell();
    if ( !state.agents_data.empty()) {
        auto predator_cell = state.agents_data[0].cell;
        if (predator_cell.location.dist(prey_cell.location)<2) {
            set_status(Agent_status::Finished); //eaten -- fail
            _episode_result = Fail;
            L("Habit_training::update_state(const State &) end");
            return;
        }
    }
    if (prey_cell == _habits[_current_habit].destination){
        set_status(Agent_status::Finished); // success
        _episode_result = Success;
        success++;
        L("Habit_training::update_state(const State &) end");
        return;
    }
    if ( _habits[_current_habit].gate_cells.contains(prey_cell)){
        set_status(Agent_status::Finished); // wrong gate -- fail
        L("Habit_training::update_state(const State &) end");
        return;
    }
    if ( _habits[_current_habit].cells.find(prey_cell)== Not_found ) {
        set_status(Agent_status::Finished); // wrong sub world
        L("Habit_training::update_state(const State &) end");
        return;
    }
    L("Habit_training::update_state(const State &) end");
}

Agent_action &Habit_training::get_action() {
    L("Habit_training::get_action() start");
    auto habit = _habits[_current_habit];
    int32_t cell_index = habit.cells.find(cell());
    uint32_t action_index;
    if (_iteration==0){ // pick a random action with probability inverse to the number of visits (exploration)
        Chance p(habit.values[cell_index].visits);
        action_index = (!p).pick();
    } else {
        action_index = _probabilities.pick(habit.values[cell_index].rewards);
    }
    _add_history(cell_index,action_index);
    L("Habit_training::get_action() end");
    return _action_set[action_index];
}

void Habit_training::end_episode(const State &) {
    L("Habit_training::end_episode(const State &) start");
    auto &habit = _habits[_current_habit];
    for (uint32_t i = 0 ; i < _iteration ; i++){
        uint32_t steps_to_end = _iteration - i ;
        habit.add_reward( _history[i].cell, _history[i].action, _rewards, _episode_result, steps_to_end);
    }
    L("Habit_training::end_episode(const State &) end");
}

void Habit_training::_add_history(uint32_t cell_index, uint32_t action_index) {
    L("Habit_training::_add_history(uint32_t , uint32_t ) start");
    while(_history.size()<=_iteration) _history.emplace_back();
    _history[_iteration].cell = cell_index;
    _history[_iteration].action = action_index;
    L("Habit_training::_add_history(uint32_t , uint32_t ) end");
}

void Habit_training::set_fixed_start(const Cell &cell) {
    L("Habit_training::set_fixed_start(const Cell &) start");
    _start = cell;
    _fixed_start = true;
    L("Habit_training::set_fixed_start(const Cell &) end");
}
