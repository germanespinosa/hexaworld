#include <agents/preys/planner.h>
#include <unistd.h>
using namespace cell_world;
using namespace std;

Planner::Planner(World &w, const Cell &start, const Cell &goal, Planning_unit pu, unsigned int planning_amount, Reward_config reward_config, unsigned int k, Paths &paths, unsigned int iterations):
        set(w.create_cell_group(), w.create_graph(), goal,reward_config, paths, iterations),
        planning_unit(pu),
        _planning_amount(planning_amount),
        _graph(w.create_graph()),
        _start(start),
        goal(goal),
        _reward_config(reward_config),
        _world(w),
        _paths(paths),
        _particle_count(k){
}


void Planner::end(Episode_result r , unsigned int l) {
    switch (_result) {
        case Success:
            set_value(_reward_config.success_reward);
            break;
        case Fail:
            set_value(_reward_config.failure_reward);
            break;
        default:
            set_value(_reward_config.default_reward);
    }
}

void Planner::update(const State &state) {
    auto &prey_cell = state.agents_data[0].cell;
    auto &predator_cell = state.agents_data[1].cell;
    bool visible = state.visible[0];
    // time to plan
    if ( visible ) {
        set.update_state(state.iteration, prey_cell.coordinates, predator_cell.coordinates);
    } else {
        set.update_state(state.iteration, prey_cell.coordinates);
    }
    // triggers planning
    set.iterations = state.iterations;
    update_state(_particle_count);
    set.create_particles(_particle_count);
    unsigned int max = 0;
    for (auto h:set.hits) if (h>max) max = h;
    for (unsigned int i =0; i<set.hits.size();i++)
        _world[i].value = (double)set.hits[i] / double(max);
    set_status(Action_ready);
}

const Cell &Planner::start(unsigned int steps) {
    set.start();
    set_goal(goal);
    return _start;
}

cell_world::Move Planner::get_move() {
    if (planning_unit == Planning_unit::milliseconds){
        if (_planning_amount) {
            Stop_watch sw;
            sw.reset();
            while (!sw.time_out(_planning_amount)) plan();
        } else
            return _paths.get_move(cell(),goal); //shortest path to the goal
    } else {
        if (_planning_amount)
            for (unsigned int i = 0; i < _planning_amount; i++) plan();
        else
            return _paths.get_move(cell(),goal); //shortest path to the goal
    }
    auto move = get_best_move();
    // add the records to the history
    return move;
}