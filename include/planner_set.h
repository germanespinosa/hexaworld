#include <agents/preys/planner_prey.h>
#include <agents/predator.h>
#include <cell_world.h>

struct Planner_set{
    Planner_set(const cell_world::World &, const cell_world::Cell &, const Reward_config &);
    cell_world::Model &get_valid_model();
    int32_t last_contact;
    cell_world::Cell_group cell_group;
    cell_world::Graph graph;
    cell_world::Model model;
    Predator predator;
    Planner_prey prey;
    std::vector<cell_world::Move> trajectory;
};