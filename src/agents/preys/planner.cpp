#include <agents/preys/planner.h>
#include <unistd.h>
using namespace cell_world;
using namespace std;

Planner::Planner(World &w, const Cell &start, const Cell &goal, Planning_unit pu, uint32_t planning_amount, Reward_config reward_config, uint32_t k, Paths &paths):
        set(w.create_cell_group(), w.create_graph(), goal,reward_config, paths),
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


void Planner::end(Episode_result r , uint32_t l, const cell_world::History &h) {
    auto &prey_cell = cell();
    cout << "{ \"winner\": " << (r == Success ? 0 : 1) << ", \"length\": " << l << ", \"reward\": " << _reward_config.value(r,l) ;
    cout << ", \"trajectories\": [";
    for (uint32_t i = 0; i<h.size(); i++){
        if (i>0) cout << ",";
        cout << "[";
        bool sep = false;
        for (auto c : h[i]){
            if (sep) cout << ",";
            cout << "["<< (int) c.x << ", "<< (int) c.y <<"]" ;
            sep = true;
        }
        cout << "]";
    }
    cout << "], \"estimated_rewards\": [ " ;
    bool sep = false;
    for (auto r : reward_history){
        if (sep) cout << ", ";
        cout << r  ;
        sep = true;
    }
    cout << "]}";
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
    reward_history.clear();
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
            for (uint32_t i = 0; i < _planning_amount; i++) plan();
        else
            return _paths.get_move(cell(),goal); //shortest path to the goal
    }
    auto move = get_best_move();
    // add the records to the history
    return move;
}