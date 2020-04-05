#pragma once
#include <cell_world.h>

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