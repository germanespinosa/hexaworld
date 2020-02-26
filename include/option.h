#pragma once
#include<cell_world.h>

struct Option : cell_world::Graph{
    Option (cell_world::Graph &, const cell_world::Cell &destination);
    const cell_world::Cell &destination;
    static std::vector<Option> get_options(cell_world::Graph &, cell_world::Graph &, cell_world::Graph &);
    static std::vector<Option> get_options(cell_world::Graph &, cell_world::Graph &);
    static std::vector<Option> get_options(cell_world::Graph &, cell_world::Cell_group &);
};

