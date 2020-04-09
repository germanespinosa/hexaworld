#include <agents/preys/planner_prey.h>

using namespace cell_world;
using namespace std;

const cell_world::Cell &Planner_prey::start(uint32_t iteration) {
    contact = false;
    current_move = 0;
    lenght = 0;
    result = Unknown;
    set_status(Action_pending);
    set_goal(goal);
    return start_cell;
}

void Planner_prey::update(const cell_world::State &state) {
    if (!state.agents_data.empty()) contact = true;
    set_status(Action_ready);
}

cell_world::Move Planner_prey::get_move() {
    return next_move;
}

Planner_prey::Planner_prey( const cell_world::Cell &g):
goal(g){}

void Planner_prey::set_start_cell(const cell_world::Cell &cell) {
    start_cell = cell;
}

void Planner_prey::set_move(const cell_world::Move &move) {
    next_move = move;
}

void Planner_prey::end(Episode_result r, uint32_t l) {
    lenght = l;
    result = r;
}
