#include <fixed_time_planner.h>

using namespace cell_world;
using namespace std;

void Fixed_time_planner::start_planning() {
    running = true;
    _thread = new thread (&Fixed_time_planner::_plan, this);
}

cell_world::Move Fixed_time_planner::get_move() {
    Move m = best_move; // copy the current best move before notifying the planner (to avoid cheating)
    running = false;
    _thread->join();
    delete (_thread);
    return m;
}

void Fixed_time_planner::plan(uint32_t iteration, Planner_history &prey_history, Planner_history &predator_history) {
    while( running );
}

Fixed_time_planner::Fixed_time_planner()
        : running(false)
        , best_move({0,0})
        , _thread (nullptr) { }

void Fixed_time_planner::_plan() {
    plan( _prey_history.last_update, _prey_history, _predator_history);
}

