#include <planner_prey.h>

using namespace cell_world;
using namespace std;

const cell_world::Cell &Planner_prey::start_episode(uint32_t iteration) {
    start_iteration = iteration;
    reward = 0;
    filtered = false;
    current_move = 0;
    set_status(Action_pending);
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
        auto prey_cell = cell();
        if ( prey_cell == goal  ) {
            reward = reward_config.value(Success, state.iteration - start_iteration);
            successes ++;
            set_status(Finished);
        } else if ( !state.agents_data.empty() && prey_cell == state.agents_data[0].cell ) {
            fails++;
            reward = reward_config.value(Fail, state.iteration - start_iteration);
            set_status(Finished);
        } else if ( state.iteration == state.iterations ) {
            unknown++;
            reward = reward_config.value(Unknown, state.iteration - start_iteration);
            set_status(Finished);
        } else {
            set_status(Action_ready);
        }
    }
}

void Planner_prey::end_episode(const cell_world::State &state) {
}

cell_world::Move Planner_prey::get_move() {
    if (current_move < trajectory.size()) return trajectory[current_move++];
    return next_move;
}

Planner_prey::Planner_prey(Graph &g):
Agent({"Planner Prey",1}),
graph(g){

}

void Planner_prey::set_start_cell(const cell_world::Cell &cell) {
    start_cell = cell;
    trajectory.clear();
}

void Planner_prey::set_move(const cell_world::Move &move) {
    next_move = move;
}

void Planner_prey::pick_random_move() {
    auto o = graph.get_connectors(cell());
    next_move = o[Chance::dice(o.size())];
}
