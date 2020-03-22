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

void Planner_set::update_state(uint32_t i, const cell_world::Cell& prey_cell, const cell_world::Cell& predator_cell){
    prey.set_start_cell(prey_cell);
    predator.set_fixed_start(predator_cell);
    last_contact = i;
    iteration = i;
}

void Planner_set::update_state (uint32_t i, const cell_world::Cell& prey_cell){
    prey.add_to_trajectory(prey_cell);
    iteration = i;
}

cell_world::Model &Planner_set::get_valid_model () {
    do {
        model.start_episode();
        model.iteration = last_contact;
        model.run(iteration);
    } while (prey.filtered);
    return model;
}