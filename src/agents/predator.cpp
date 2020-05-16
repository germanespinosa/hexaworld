#include <agents/predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;


static uint32_t _randomness = 0;

Predator::Predator(Graph &graph, Graph &visibility, cell_world::Paths &paths)
    : _fixed_start (false)
    , _contact(false)
    , _chasing (false)
    , _graph(graph)
    , _next_move({0,0})
    , _prev_move({0,0})
    , _inverted_visibility(visibility.invert())
    , _map(_graph.nodes)
    , _paths(paths)
    , Agent({"predator", 1}) {
}

const Cell &Predator::start_episode(uint32_t) {
    _prev_move = Move{0,0};
    set_color(Blue);
    _chasing = false;
    data.icon = Icon::Predator_icon;
    if (_fixed_start) return _start;
    return _graph.nodes.random_cell();
}

void Predator::update_state(const State &state) {
    auto predator_cell = cell();

    if ( !state.agents_data.empty() ) {
        auto prey_cell = state.agents_data[0].cell;
        _chasing = true;
        _last_prey_cell = prey_cell;
        _contact = true;
    } else {
        _contact = false;
        if (predator_cell == _last_prey_cell) _chasing = false;
    }

    if (Chance::dice(100)>_randomness){
        if (!_chasing) {
            _last_prey_cell = _inverted_visibility[predator_cell].random_cell();
        }
        auto nc = cell();
        _next_move = _paths.get_move(nc,_last_prey_cell);
        if (Chance::dice(2))
        {
            nc = _map [nc.coordinates+_next_move];
            _next_move += _paths.get_move(nc,_last_prey_cell);
        }
    }else {
        Connection_pattern cp = Connection_pattern::get_pattern(cell(), _graph[predator_cell]);
        do {
            _next_move = cp.random_move();
        } while ( _next_move == _prev_move && cp.size()>1); //going backwards and there are options
    }

    if (_chasing)
        set_color(Yellow);
    else
        set_color(Blue);

    set_status(Action_ready);
}

Move Predator::get_move() {
    _prev_move = -_next_move;
    return _next_move;
}

void Predator::end_episode(const State &state, const cell_world::History &) {

}

void Predator::set_fixed_start(const Cell &cell) {
    _fixed_start = true;
    _start = cell;
}

void Predator::set_random_start() {
    _fixed_start = false;
}

void Predator::set_randomness(uint32_t randomness) {
    _randomness = randomness;
}
