#pragma once
#include <cellworld.h>
#include <action_set.h>
#include "hexaworld.h"

struct Habit_value{
    Habit_value(uint32_t);
    std::vector<double> rewards;
    uint32_t visits;
    double lenght;
    uint32_t policy();
};

struct Habit{
    uint32_t sub_world_id;
    cell_world::Cell destination;
    cell_world::Cell_group cells;
    cell_world::Cell_group gate_cells;
    std::vector<Habit_value> values;
    bool convergence;
    bool load(const std::string&, uint32_t);
    bool save(const std::string&);
private:
    const std::string _extension = ".habit";
};
