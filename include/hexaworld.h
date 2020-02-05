#pragma once
#include<cellworld.h>

#define CONTACT_CELLS {{{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}}}

#define ADJACENT_CELLS cell_world::Connection_pattern({{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}})
#define PREDATOR_PROBABILITIES std::vector<uint32_t>{10,1,1,1,1,1}

enum Episode_result{
    Fail,
    Success,
    Unknown
};

struct Reward_config{
    double success_reward;
    double failure_reward;
    double default_reward;
    double discount;
    double step_cost;
    double value (Episode_result, uint32_t);
};