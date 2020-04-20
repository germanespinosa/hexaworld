#include <agents/prey.h>

using namespace std;
using namespace cell_world;

void Prey::update_state(const State &s) {
    if (!_ready) throw logic_error("Goal not set");
    if (_eaten){
        return;
    }
    if (cell() == _goal){
        end (Success, lenght);
        set_status(Finished);
        return;
    }
    if (s.iteration == s.iterations){
        end (Unknown, lenght);
        set_status(Finished);
        return;
    }
    update(s);
    lenght++;
}

const Cell &Prey::start_episode(uint32_t i) {
    _eaten = false;
    _ready = false;
    _start_iteration = i;
    lenght = 0;
    return start(i);
}

void Prey::receive_message(const Agent_message &m) {
    if (m.from.name == "predator") {
        _eaten = true;
        end (Fail, lenght);
        set_status(Finished);
    }
}

void Prey::end_episode(const cell_world::State &) {
}

Prey::Prey(): Agent({"prey",1}) {
    set_color(Green);
    data.icon = Icon::Prey_icon;
}

void Prey::update(const cell_world::State &) {}

void Prey::set_goal(const cell_world::Cell &g) {
    _ready = true;
    _goal = g;
}

void Prey::end(Episode_result, uint32_t) {

}
