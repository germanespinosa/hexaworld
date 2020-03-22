#include <fixed_time_prey.h>

using namespace std;
using namespace cell_world;

Fixed_time_prey::Fixed_time_prey(uint32_t steps, Planner &planner, double time, const cell_world::Cell &start, const cell_world::Cell &goal):
_steps(steps),
_planner(planner),
_time_out(time),
_start(start),
_goal(goal),
Agent ({"prey",1}){
    L("Fixed_time_prey::Fixed_time_prey start");

}

const Cell &Fixed_time_prey::start_episode() {
    L("Fixed_time_prey::start_episode() start");
    L("Fixed_time_prey::start_episode() end");
    return _start;
}

void Fixed_time_prey::update_state(const State &state) {
    if (cell()==_goal) {
        set_status(Finished);
    } else if (!state.agents_data.empty() && state.agents_data[0].cell == cell()) {
        set_status(Finished);
    } else if (!_planner.planning) {
        // time to plan
        // add the records to the history
        _planner.planning = true;
        if (!state.agents_data.empty())
            _planner.set.update_state(state.iteration,cell(),state.agents_data[0].cell);
        else
            _planner.set.update_state(state.iteration,cell());
        // triggers the planning
        _stop_watch.reset(); // starts the clock
    }else if ( _stop_watch.time_out(_time_out)) {
        // stop planning and get the result
        _stop_watch.stop();
        _planner.planning = false;
        set_status(Action_ready);
    }
}

Move Fixed_time_prey::get_move() {
    return _planner.get_move();
}

void Fixed_time_prey::end_episode(const cell_world::State &) {

}