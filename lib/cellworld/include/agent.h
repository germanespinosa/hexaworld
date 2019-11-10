#pragma once
#include <core.h>

namespace cellworld{
     struct Agent_data{
        uint32_t id;
        uint32_t iteration;
        Coordinates coordinates;
        uint8_t status;
    };
     struct State{
        uint32_t iteration;
        std::vector<Agent_data> agents;
    };
    struct Action{
        uint32_t iteration;
        Coordinates action;
    };
    struct Agent{
         virtual void update_state(State);
         virtual bool get_action() ;
         Action last_action;
         Agent_data data;
    };
}  