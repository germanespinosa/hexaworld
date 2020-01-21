#pragma once
#include <cell_world.h>
#include <hexaworld.h>

struct Action_set{
    Action_set(cell_world::Cell_group &,cell_world::Connection_pattern, cell_world::Probabilities &);
    cell_world::Agent_action &operator[](uint32_t);
    uint32_t  get_best (cell_world::Location);
    uint32_t size();
private:
    std::vector<cell_world::Agent_action> _actions;
    std::vector<cell_world::Location> _displacements;
    void _add_action(cell_world::Location, cell_world::Cell_group &,  cell_world::Probabilities &);
};
