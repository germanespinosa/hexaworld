#pragma once
#include <ge211.h>
#include <core.h>
#include <world.h>
#include <agent.h>

namespace cellworld{
    struct Model
    {
        Model( World &world, std::vector<Agent>&);
        World &world;
        bool update();
        private:
    // private members
        uint32_t _iteration;
        std::vector<Agent> &_agents;
        void _epoch (std::vector<Action>);
    };
}