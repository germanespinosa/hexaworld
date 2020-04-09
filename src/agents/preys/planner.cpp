#include <agents/preys/planner.h>
#include <unistd.h>
using namespace cell_world;
using namespace std;

Planner::Planner(World &w, const Cell &start, const Cell &goal, double interval, Reward_config reward_config):
        set(w, goal,reward_config),
        _graph(w.create_graph()),
        _start(start),
        goal(goal),
        _interval(interval),
        _reward_config(reward_config),
        _world(w){
}

void Planner::_planning_job(){
    _running = true;
    while(_running){
        if (status == Action_pending) {
            Stop_watch sw;
            sw.reset();
            for (uint32_t i=0; i<5000 && _running; i++)
            //while (!sw.time_out(_interval) && )
            {
                plan();
            }
            set_status(Action_ready);
        }
    }
    _running = true;
}

void Planner::end(Episode_result, uint32_t) {
    _running = false;
    _thread->join();
}

void Planner::update(const State &state) {
    auto &prey_cell = cell();
    cout << "prey: " << prey_cell.coordinates;
    if (!state.agents_data.empty()){
        cout << " predator: " << state.agents_data[0].cell.coordinates << " " << (state.agents_data[0].cell == prey_cell);
    }
    cout << endl;
    // time to plan
    if (!state.agents_data.empty()) {
        set.trajectory.clear();
        set.predator.set_fixed_start(state.agents_data[0].cell);
        set.prey.set_start_cell(prey_cell);
        set.last_contact = state.iteration;
    } else {
        set.iteration = state.iteration;
    }
    // triggers the planning
    set.model.iterations = state.iterations;
    //_mutex.lock();
    update_state();
    set_status(Action_pending);
    //_mutex.unlock();
}

const Cell &Planner::start(uint32_t steps) {
    set.prey.set_start_cell(_start);
    set.iteration = 0;
    set.model.iterations = steps;
    _thread = new std::thread(&Planner::_planning_job, this);
    set_goal(goal);
    return _start;
}

cell_world::Move Planner::get_move() {
    auto move = get_best_move();
    // add the records to the history
    set.trajectory.push_back(move);
    cout << "best move " <<  move <<  endl;
    return move;
}