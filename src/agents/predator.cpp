#include <predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;

Predator::Predator(cell_world::World &world, const cell_world::Probabilities& probabilities)
    : _visits (world.size())
    , _use_view_range(false)
    , _fixed_start (false)
    , _chasing (false)
    , _cg(world.create_cell_group())
    , _next_action(Not_found)
    , _random_action(world.connection_pattern,Probabilities(vector<uint32_t>((world.connection_pattern).size(),1)))
    , _probabilities(probabilities)
    , _actions(_cg,world.connection_pattern,_probabilities)
    , _first_episode(true)
    , Agent({"Predator", 1}) {
    for (uint32_t i = 0; i < world.size(); i++) _visits[i] = world[i].occluded ? 0 :1;
}

const Cell &Predator::start_episode(const State &state) {
    set_color(Green);
    if (_fixed_start) return _start;
    Probabilities p(_visits);// start in a cell with probability proportional to previous visits
    set_status(Action_ready);
    return _cg[p.pick()];
}

void Predator::update_state(const cell_world::State &state) {
    auto predator_cell = cell();
    _visits[_cg.find(predator_cell)]++;
    set_color(Green);
    set_status(Action_ready);
    if ( !state.agents_data.empty() ) {
        auto prey_cell = state.agents_data[0].cell;
        if ( !_use_view_range || predator_cell.location.dist(prey_cell.location) < _view_range ) {
            set_color(Red);
            _chasing = true;
            _last_prey_cell = prey_cell;
        }
    }
    if (predator_cell == _last_prey_cell) _chasing = false;

    if (_chasing){
        auto displacement = _last_prey_cell.location - predator_cell.location ;
        _next_action = _actions.get_best(displacement);
    }else {
        _next_action = Not_found;
    }
}

cell_world::Agent_action &Predator::get_action() {
    if (_next_action == Not_found)
        return _random_action;
    else
        return _actions[_next_action];
}

void Predator::end_episode(const cell_world::State &state) {

}

void Predator::set_fixed_start(const cell_world::Cell &cell) {
    _fixed_start = true;
    _start = cell;
}

void Predator::set_view_range(double range) {
    if (range <=0 ) _use_view_range = false;
    else {
        _use_view_range = true;
        _view_range = range;
    }
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
    set_status(Action_ready);
    return _cg[i];
}

void Test_prey::update_state(const cell_world::State &state) {
    set_status(Action_ready);
}

cell_world::Agent_action &Test_prey::get_action() {
    return _random_action;
}

void Test_prey::end_episode(const cell_world::State &) {

}