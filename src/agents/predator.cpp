#include <predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;

Predator::Predator(Cell_group cell_group, cell_world::Visibility &visibility)
    : _chasing (false)
    , _cg(std::move(cell_group))
    , _next_action(Not_found)
    , _prey_index(Not_found)
    , _random_action(ADJACENT_CELLS,Probabilities(vector<uint32_t>((ADJACENT_CELLS).size(),1)))
    , _probabilities(PREDATOR_PROBABILITIES)
    , _actions(cell_group,ADJACENT_CELLS,_probabilities)
    , _visibility (visibility)
    , Agent({"Predator", 1}) {

}

const Cell &Predator::start_episode(const State &state) {
    _prey_index = state.find("Prey");
    if (_prey_index== Not_found){
        set_color(Yellow);
    } else {
        set_color(Blue);
    }
    uint32_t i = 0;
    while (_cg[ i = (rand() % _cg.size())].occluded);
    _iteration = 0;
    return _cg[i];
}

void Predator::update_state(const cell_world::State &state) {
    _iteration = state.iteration;
    if ( _prey_index != Not_found ) {
        auto predator_cell = cell();
        auto prey_cell = state.agents_data[_prey_index].cell;
        if (_visibility[predator_cell].contains(prey_cell)) {
            _chasing = true;
            _last_prey_cell = prey_cell;
        }
        if (predator_cell == _last_prey_cell) _chasing = false;
        if (_chasing){
            auto displacement = _last_prey_cell.location - predator_cell.location ;
            _next_action = _actions.get_best(displacement);
            set_color(Purple);
        }else {
            _next_action = Not_found;
            set_color(Blue);
        }
    }
}

uint32_t Predator::action_ready() {
    return _iteration;
}

cell_world::Agent_action &Predator::get_action() {
    if (_next_action == Not_found)
        return _random_action;
    else
        return _actions[_next_action];
}

void Predator::end_episode(const cell_world::State &state) {

}

Test_prey::Test_prey(cell_world::Cell_group cg) :
    _cg(cg),
    _random_action(ADJACENT_CELLS,Probabilities(vector<uint32_t>((ADJACENT_CELLS).size(),1))),
    Agent({"Prey",1}){
    set_color(Green);
}

const cell_world::Cell &Test_prey::start_episode(const cell_world::State &) {
    uint32_t i = 0;
    while (_cg[ i = (rand() % _cg.size())].occluded);
    _iteration = 0;
    return _cg[i];
}

void Test_prey::update_state(const cell_world::State &state) {
    _iteration = state.iteration;
}

uint32_t Test_prey::action_ready() {
    return _iteration;
}

cell_world::Agent_action &Test_prey::get_action() {
    return _random_action;
}

void Test_prey::end_episode(const cell_world::State &) {

}