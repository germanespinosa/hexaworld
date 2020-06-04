#include <agents/predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;


static uint32_t _randomness = 0;

Predator::Predator(Graph &graph, Graph &visibility, cell_world::Paths &paths)
    : _fixed_start (false)
    , _contact(false)
    , _graph(graph)
    , _next_move({0,0})
    , _prev_move({0,0})
    , _visibility(visibility)
    , _inverted_visibility(_visibility.invert())
    , _map(_graph.nodes)
    , _paths(paths)
    , Agent({"predator", 1}) {
}

const Cell &Predator::start_episode(uint32_t) {
    _prev_move = Move{0,0};
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
    else
        cout << "ERROR" << endl;
    return s;
}

void Predator::update_state(const State &state) {
    auto predator_cell = cell();

    if ( !state.agents_data.empty() ) {
        auto prey_cell = state.agents_data[0].cell;
        _last_prey_cell = prey_cell;
        _contact = true;
    } else {
        _contact = false;
        if (_visibility[predator_cell].contains(_last_prey_cell))
            _last_prey_cell = _inverted_visibility[predator_cell].random_cell();
    }
    if (Chance::dice(100)>_randomness){
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

    if (_contact)
        set_color(Yellow);
    else
        set_color(Blue);

    set_status(Action_ready);
    _last_prey_cell = _graph[_last_prey_cell].random_cell();
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
