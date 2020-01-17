#pragma once
#include<cellworld.h>

#define CONTACT_CELLS {{{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}}}

#define ADJACENT_CELLS cell_world::Connection_pattern({{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}})
#define PREDATOR_PROBABILITIES std::vector<uint32_t>{10,1,1,1,1,1}

struct Action_set{
    std::vector<cell_world::Agent_action> actions;
    std::vector<cell_world::Location> displacements;
    bool save(const std::string&);
    bool load(const std::string&);
private:
    const std::string _extension = ".action_set";
};
