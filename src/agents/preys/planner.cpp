#include <agents/preys/planner.h>
#include <unistd.h>
using namespace cell_world;
using namespace std;

Planner::Planner(World &w, const Cell &start, const Cell &goal, double interval, Reward_config reward_config):
        set(w, goal,reward_config),
        planning_iterations(5000),
        _graph(w.create_graph()),
        _start(start),
        goal(goal),
        _interval(interval),
        _reward_config(reward_config),
        _world(w),
        _running(true){
}

void Planner::_planning_job(){
    while(_running){
        if (status == Action_pending) {
            Stop_watch sw;
            sw.reset();
            for (uint32_t i = 0; i < planning_iterations && _running; i++)
            //while (!sw.time_out(_interval) && )
                plan();
            set_status(Action_ready);
        }
    }
}

void Planner::end(Episode_result r , uint32_t l) {
    _running = false;
    _thread->join();
    cout << (r == Success ? 1 : 0) << ", " << l << ", " << _reward_config.value(r,l);
}

void Planner::update(const State &state) {
    auto &prey_cell = cell();
    // time to plan
    if (!state.agents_data.empty()) {
        set.trajectory.clear();
        set.predator.set_fixed_start(state.agents_data[0].cell);
        set.prey.set_start_cell(prey_cell);
        set.last_contact = state.iteration;
    }
    // triggers the planning
    set.model.iterations = state.iterations;
    update_state();
    set_status(Action_pending);
}

const Cell &Planner::start(uint32_t steps) {
    set.prey.set_start_cell(_start);
    set.model.iterations = steps;
    _thread = new std::thread(&Planner::_planning_job, this);
    set_goal(goal);
    return _start;
}

cell_world::Move Planner::get_move() {
    auto move = get_best_move();
    // add the records to the history
    set.trajectory.push_back(move);
    return move;
}