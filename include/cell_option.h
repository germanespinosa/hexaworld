#pragma once
#include <cell_world.h>
#include <habit.h>

struct Policy_set{
    explicit Policy_set (Habit &);
    const cell_world::Cell &destination;
    std::vector<cell_world::Move> policies;
    cell_world::Cell_group _cells;
    cell_world::Move operator[](const cell_world::Cell &);
};

struct Cell_option{
    Cell_option (const cell_world::Cell &, Policy_set &, uint32_t);
    const cell_world::Cell &cell;
    uint32_t length;
    Policy_set &policy_set;
};

struct Cell_options{
    Cell_options(const std::string &);
    cell_world::World world;
    cell_world::Cell_group gates;
    cell_world::Graph world_graph;
    cell_world::Graph gates_graph;
    cell_world::Graph options_graph;
    std::vector<std::vector<Cell_option>> cell_options; //one per cel, per cell-option
    std::vector<Policy_set> policy_sets; // one per world-option
    std::vector<Habit> habits;
    std::vector<Cell_option> operator[](const cell_world::Cell &);
};