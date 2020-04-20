#include <agents/preys/planner.h>
#include <unistd.h>
using namespace cell_world;
using namespace std;

Planner::Planner(World &w, const Cell &start, const Cell &goal, double planning_time, Reward_config reward_config, uint32_t k):
        set(w.create_cell_group(),w.create_graph(), goal,reward_config),
        _planning_iterations(Not_found),
        _graph(w.create_graph()),
        _start(start),
        goal(goal),
        _planning_time(planning_time),
        _reward_config(reward_config),
        _world(w),
        _particle_count(k){
}

Planner::Planner(World &w, const Cell &start, const Cell &goal, uint32_t planning_iterations, Reward_config reward_config, uint32_t k):
        set(w.create_cell_group(),w.create_graph(), goal,reward_config),
        _planning_iterations(planning_iterations),
        _graph(w.create_graph()),
        _start(start),
        goal(goal),
        _planning_time(0),
        _reward_config(reward_config),
        _world(w),
        _particle_count(k){
}


void Planner::end(Episode_result r , uint32_t l) {
    cout << (r == Success ? 1 : 0) << ", " << l << ", " << _reward_config.value(r,l);
}

void Planner::update(const State &state) {
    auto &prey_cell = cell();
    // time to plan
    if (!state.agents_data.empty()) {
        set.update_state(state.iteration, data.cell.coordinates, state.agents_data[0].cell.coordinates);
    } else {
        set.update_state(state.iteration, data.cell.coordinates);
    }
    // triggers planning
    set.iterations = state.iterations;
    update_state(_particle_count);
    set.create_particles(_particle_count);
    uint32_t max = 0;
    for (auto h:set.hits) if (h>max) max = h;
    for (uint32_t i =0; i<set.hits.size();i++)
        _world[i].value = (double)set.hits[i] / double(max);
    set_status(Action_ready);
}

const Cell &Planner::start(uint32_t steps) {
    set_goal(goal);
    return _start;
}

cell_world::Move Planner::get_move() {
    if (_planning_iterations == Not_found){
        Stop_watch sw;
        sw.reset();
        while (!sw.time_out(_planning_time)) plan();
    } else {
        for (uint32_t i = 0; i < _planning_iterations; i++) plan();
    }
    auto move = get_best_move();
    // add the records to the history
    return move;
}