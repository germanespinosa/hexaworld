#include <planner_prey.h>
#include <predator.h>
#include <cell_world.h>
#include <planner_history.h>

struct Planner_set{
    Planner_set(const cell_world::World &);
    cell_world::Model &get_valid_model();
    uint32_t last_contact;
    uint32_t iteration;
    const cell_world::World &world;
    cell_world::Cell_group cell_group;
    cell_world::Graph graph;
    cell_world::Model model;
    Predator predator;
    Planner_prey prey;
    std::vector<cell_world::Move> trajectory;
};