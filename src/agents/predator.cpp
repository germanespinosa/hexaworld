#include <agents/predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;


static unsigned int _randomness = 0;

Predator::Predator(Graph &graph, Graph &visibility, cell_world::Paths &paths, const Reward_config &rc, const cell_world::Cell &goal)
    : _reward_config(rc)
    , _fixed_start (false)
    , _graph(graph)
    , _next_move({0,0})
    , _prev_move({0,0})
    , _visibility(visibility)
    , _inverted_visibility(_visibility.invert())
    , _map(_graph.nodes)
    , _paths(paths)
    , _goal(goal)
    , Agent({"predator", 1}) {
}

const Cell &Predator::start_episode(unsigned int) {
    _prev_move = Move{0,0};
    set_value(0);
    set_color(Blue);
    data.icon = Icon::Predator_icon;
    if (_fixed_start) {
        if (_inverted_visibility[_start].size()>0)
            _last_prey_cell = _inverted_visibility[_start].random_cell();
        return _start;
    }
    auto &s = _graph.nodes.random_cell();
    if (_inverted_visibility[s].size()>0)
        _last_prey_cell = _inverted_visibility[s].random_cell();
    return s;
}

void Predator::update_state(const State &state) {
    set_status(Action_ready);
}

Move Predator::get_move(const State &state) {
    auto &prey_cell = state.agents_data[0].cell;
    auto &predator_cell = state.agents_data[1].cell;
    bool visible = state.visible[0];

    if (prey_cell==_goal) {
        _next_move = Move{0,0};
        set_value(0);
        set_status(Action_ready);
        return {0,0};
    }

    if ( visible ) {
        _last_prey_cell = prey_cell;
    } else {
        //_last_prey_cell = _graph[_last_prey_cell].random_cell();
        //if (_visibility[predator_cell].contains(_last_prey_cell)) {
        if (predator_cell == _last_prey_cell) {
            _last_prey_cell = _inverted_visibility[predator_cell].random_cell();
            set_value(0);
        }
    }
    auto destination = predator_cell;
    if (Chance::dice(100)>=_randomness){
        _next_move = _paths.get_move(destination,_last_prey_cell);
        if (Chance::dice(2))
        {
            destination = _map [predator_cell.coordinates + _next_move];
            _next_move += _paths.get_move(destination,_last_prey_cell);
        }
        destination = _map [predator_cell.coordinates + _next_move];
    }else {
        do {
            destination = _graph[predator_cell].random_cell();
            _next_move = destination.coordinates - predator_cell.coordinates;
        } while ( _next_move == _prev_move && _graph[predator_cell].size()>1); //going backwards and there are options
    }
    if (prey_cell==destination) {
        set_value(_reward_config.success_reward);
        set_status(Action_ready);
        return {0,0};
    }
    if (visible) {
        double distance_to_prey = destination.coordinates.manhattan(predator_cell.coordinates);
        double distance_prey_to_goal = prey_cell.coordinates.manhattan(_goal.coordinates);
        set_value (_reward_config.success_reward * distance_prey_to_goal / ( distance_prey_to_goal + distance_to_prey));
    } else {
        set_value (value * .9);
    }
    _prev_move = -_next_move;
    return _next_move;
}

void Predator::end_episode(const State &state) {

}

void Predator::set_fixed_start(const Cell &cell) {
    _fixed_start = true;
    _start = cell;
}

void Predator::set_random_start() {
    _fixed_start = false;
}

void Predator::set_randomness(unsigned int randomness) {
    _randomness = randomness;
}
