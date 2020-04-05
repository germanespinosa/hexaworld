#include <planner_set.h>

using namespace cell_world;
using namespace std;

Planner_set::Planner_set(const cell_world::World &w):
        world(w),
        cell_group(w.create_cell_group()),
        graph(w.create_graph()),
        model(cell_group),
        prey(graph),
        predator(graph),
        last_contact(0),
        iteration(0){
    model.add_agent(prey);
    model.add_agent(predator);
}

cell_world::Model &Planner_set::get_valid_model () {
    if (model.status == cell_world::Model::Status::Running) model.end_episode();
    do {
        model.start_episode(last_contact);
        uint32_t move_index = 0;
        for (uint32_t move_index = 0; move_index < trajectory.size() && !prey.filtered ;move_index++) {
            prey.set_move(trajectory[move_index]);
            if (!model.update()) break;
        }
        if (prey.filtered) model.end_episode();
    } while (prey.filtered);
    return model;
}