#include <fixed_time_prey.h>

using namespace std;
using namespace cell_world;

Fixed_time_prey::Fixed_time_prey(Fixed_time_planner &planner, double time, const cell_world::Cell &start, const cell_world::Cell &goal):
_planner(planner),
_time_out(time),
_start(start),
_goal(goal),
Agent ({"prey",1}){

}

const Cell &Fixed_time_prey::start_episode() {
    _planner._prey_history.clear();
    return _start;
}

void Fixed_time_prey::update_state(const State &state) {
    if (cell()==_goal) {
        set_status(Finished);
    } else if (!state.agents_data.empty() && state.agents_data[0].cell == cell()) {
        set_status(Finished);
    } else if (!_planner.running) {
        // time to plan
        // add the records to the history
        _planner._prey_history.add_record(state.iteration,cell());
        if (!state.agents_data.empty()) _planner._predator_history.add_record(state.iteration,state.agents_data[0].cell);
        // triggers the planning
        _planner.start_planning();
        _stop_watch.reset(); // starts the clock
    }else if (_stop_watch.time_out(_time_out)) {
        _stop_watch.stop();
        set_status(Action_ready);
    }
}

Move Fixed_time_prey::get_move() {
    return _planner.get_move();
}

void Fixed_time_prey::end_episode(const cell_world::State &) {

}