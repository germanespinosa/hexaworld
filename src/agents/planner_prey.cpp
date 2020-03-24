#include <planner_prey.h>

using namespace cell_world;
using namespace std;

const cell_world::Cell &Planner_prey::start_episode(uint32_t) {
    filtered = false;
    current_move = 0;
    return start_cell;
}

void Planner_prey::update_state(const cell_world::State &state) {
    if (current_move < trajectory.size()) {
        if (!state.agents_data.empty())
        {
            // invalid state was generated
            filtered = true;
            set_status(Finished);
            return;
        } else {
            set_status(Action_ready);
        }
    } else {
        set_status(Action_ready);
    }
}

void Planner_prey::end_episode(const cell_world::State &state) {
}

cell_world::Move Planner_prey::get_move() {
    if (current_move < trajectory.size()) return trajectory[current_move++];
    return next_move;
}

Planner_prey::Planner_prey(Graph &g):
Agent({"Prey",1}),
graph(g){

}

void Planner_prey::set_start_cell(const cell_world::Cell &cell) {
    start_cell = cell;
    current_cell = cell;
    trajectory.clear();
}

void Planner_prey::add_to_trajectory(const cell_world::Cell &cell) {
    trajectory.push_back(cell.coordinates - current_cell.coordinates);
    current_cell = cell;
}

void Planner_prey::set_move(const cell_world::Move &move) {
    next_move = move;
}

std::vector<cell_world::Move> Planner_prey::get_options() {
    std::vector<cell_world::Move> options;
    auto &cc = graph[cell()];
    for (uint32_t i=0;i<cc.size();i++) options.push_back( cc[i].coordinates - cell().coordinates );
    return options;
}

void Planner_prey::pick_random_move() {
    auto o = get_options();
    next_move = o[Chance::dice(o.size())];
}
