#include <agents/predator.h>
#include <iostream>

using namespace cell_world;
using namespace std;

Predator::Predator(Graph &graph)
    : _use_view_range(false)
    , _fixed_start (false)
    , _chasing (false)
    , _graph(graph)
    , _first_episode(true)
    , Agent({"predator", 1}) {
}

const Cell &Predator::start_episode(uint32_t) {
    L("Predator::start_episode(const State &) start");
    _chasing = false;
    set_color(Blue);
    set_status(Action_ready);
    data.icon = Icon::Predator_icon;
    if (_fixed_start) return _start;
    auto &cell = _graph.nodes[Chance::dice(_graph.nodes.size())];
    Coordinates c = {1,7};
    L("Predator::start_episode(const State &) end");
    return cell;
}

void Predator::update_state(const State &state) {
    L("Predator::update_state(const cell_world::State &state) start");
    auto predator_cell = cell();
    set_color(Blue);
    _contact = false;
    if ( !state.agents_data.empty() ) {
        auto prey_cell = state.agents_data[0].cell;
        if ( !_use_view_range || predator_cell.location.dist(prey_cell.location) < _view_range ) {
            set_color(Yellow);
            _chasing = true;
            _last_prey_cell = prey_cell;
            _contact = true;
        }
    }
    if (predator_cell == _last_prey_cell) _chasing = false;

    if (_chasing){
        auto nc = cell();
        auto dice = Chance::dice(2);
        do {
            auto c = nc;
            vector<double> distances = _graph[c].get_distances(_last_prey_cell);
            uint32_t mini = 0;
            for (uint32_t i = 1; i < distances.size(); i++) if (distances[i] < distances[mini]) mini = i;
            nc = _graph[c][mini];
            if (_contact && _last_prey_cell == nc){
                send_message({"prey",0},"eaten");
                set_status(Action_pending);
                return;
            }
        } while (dice--);
        _next_move = nc.coordinates-cell().coordinates;
    }else {
        auto &con = _graph[cell()];
        Connection_pattern cp = Connection_pattern::get_pattern(cell(),con);
        _next_move = cp[Chance::dice(cp.size())];
    }

    set_status(Action_ready);
    L("Predator::update_state(const cell_world::State &state) end");
}

Move Predator::get_move() {
    return _next_move;
}

void Predator::end_episode(const State &state) {

}

void Predator::set_fixed_start(const Cell &cell) {
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

void Predator::set_random_start() {
    _fixed_start = false;
}
