#include <planner.h>

using namespace cell_world;
using namespace std;

Planner::Planner(const World &w, const Cell &start, const Cell &goal, double interval):
        set(w),
        _start(start),
        _goal(goal),
        _interval(interval),
        //_thread(&Planner::_planning_job, this),
        Agent({"Prey",1}){
}

Planner::~Planner(){
    _running = false;
    //_thread.join();
    _clock.reset();
}

void Planner::_planning_job(){
    _running = true;
    cout << "starting planning job " << _clock.elapsed() << endl;
    while(_running){
        if (data.status == Action_pending) {
            if (!_stop_watch.time_out(_interval)) {
                plan();
            } else {
                cout << "done planning: " << _clock.elapsed() << endl;
                set_status(Action_ready);
            }
        } else {
        }
    }
}

void Planner::end_episode(const cell_world::State &) {

}

void Planner::update_state(const State &state) {
    if (cell()==_goal) {
        set_status(Finished);
    } else if (!state.agents_data.empty() && state.agents_data[0].cell == cell()) {
        set_status(Finished);
    } else {
        // time to plan
        // add the records to the history
        if (!state.agents_data.empty())
            set.update_state(state.iteration,cell(),state.agents_data[0].cell);
        else
            set.update_state(state.iteration,cell());
        update_state();
        // triggers the planning
        cout << "start planning: " << _clock.elapsed() << endl;
        _stop_watch.reset(); // starts the clock
    }
}


const Cell &Planner::start_episode(uint32_t steps) {
    cout << "start episode: " << _clock.elapsed() << endl;
    _iterations = steps;
    return _start;
}