#pragma once
#include <core.h>

namespace cellworld{
     struct State{
        uint32_t iteration;
//        std::vector<Agent> agents;
    };
    struct Action{
        uint32_t iteration;
        Coordinates action;
    };
    struct Agent{
         void update_state(State);
         bool get_action();
         Action last_action;
         Coordinates coordinates;
    };
} 