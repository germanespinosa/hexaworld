#include <agents/predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;


static uint32_t _randomness = 0;

Predator::Predator(Graph &graph)
    : _fixed_start (false)
    , _contact(false)
    , _chasing (false)
    , _graph(graph)
    , _next_move({0,0})
    , _prev_move({0,0})
    , Agent({"predator", 1}) {
}

const Cell &Predator::start_episode(uint32_t) {
    _prev_move = Move{0,0};
    set_color(Blue);
    _chasing = false;
    data.icon = Icon::Predator_icon;
    if (_fixed_start) return _start;
    return _graph.nodes[Chance::dice(_graph.nodes.size())];
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
    }

    if (predator_cell == _last_prey_cell) _chasing = false;

    if (_chasing && Chance::dice(100)>_randomness){
        auto nc = cell();
        auto dice = Chance::dice(2);
        do {
            auto c = nc;
            vector<double> distances = _graph[c].get_distances(_last_prey_cell);
            uint32_t mini = 0;
            for (uint32_t i = 1; i < distances.size(); i++) if (distances[i] < distances[mini]) mini = i;
            nc = _graph[c][mini];
        } while (dice-- && !(_contact && _last_prey_cell == nc));
        _next_move = nc.coordinates-cell().coordinates;
    }else {
        auto &con = _graph[cell()];
        Connection_pattern cp = Connection_pattern::get_pattern(cell(), con);
        do {
            _next_move = cp[Chance::dice(cp.size())];
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
