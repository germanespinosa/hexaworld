#include <reward_config.h>

using namespace std;

double Reward_config::value(Episode_result result, uint32_t steps) {
    double reward = 0;
    switch (result){
        case Success:
            reward = success_reward;
            break;
        case Fail:
            reward = failure_reward;
            break;
        default:
            reward = default_reward;
    }
    double v = (reward + step_cost * steps) * pow(discount,steps) ;
    return v;
}

