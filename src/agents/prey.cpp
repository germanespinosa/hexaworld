#include <agents/prey.h>

using namespace std;
using namespace cell_world;

void Prey::update_state(const State &s) {
    if (!_ready) throw logic_error("Goal not set");
    auto &prey_cell = s.agents_data[0].cell;
    auto &predator_cell = s.agents_data[1].cell;

    if (prey_cell == _goal){
        _result = Success;
        set_status(Finished);
        return;
    }
    if (prey_cell == predator_cell){
        _result = Fail;
        set_status(Finished);
        return;
    }
    if (s.iteration == s.iterations){
        _result = Unknown;
        set_status(Finished);
        return;
    }
    update(s);
    lenght++;
}

const Cell &Prey::start_episode(uint32_t i) {
    _ready = false;
    _result = Unknown;
    _start_iteration = i;
    lenght = 0;
    return start(i);
}

void Prey::end_episode(const cell_world::State &) {
    end (_result, lenght);
}

Prey::Prey():
    Agent({"prey",1}),
    lenght(0),
    _ready(false),
    _start_iteration(0){
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
