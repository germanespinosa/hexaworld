#include <planner_set.h>

using namespace cell_world;
using namespace std;

Planner_set::Planner_set(const cell_world::World &w, const Cell &goal, const Reward_config &rc ):
        cell_group(w.create_cell_group()),
        graph(w.create_graph()),
        model(cell_group),
        prey(goal),
        predator(graph),
        last_contact(Not_found){
    model.add_agent(predator);
    model.add_agent(prey);
}

cell_world::Model &Planner_set::get_valid_model () {
    uint32_t start_iteration = 0;
    if (last_contact != Not_found) start_iteration = last_contact;

    if (model.status == cell_world::Model::Status::Running) model.end_episode();
    if (trajectory.empty()) {
        model.start_episode(start_iteration);
    } else {
        bool filtered;
        do {
            filtered = false;
            model.start_episode(start_iteration);
            if (last_contact != Not_found) prey.contact = false;
            for (uint32_t move_index = 0; move_index < trajectory.size() && !prey.contact; move_index++) {
                prey.set_move(trajectory[move_index]);
                if (!model.update()) {
                    model.end_episode();
                    filtered = true;
                    break;
                }
            }
            if (prey.contact) {
                model.end_episode();
                filtered = true;
            }
        } while (filtered);
    }
    return model;
}