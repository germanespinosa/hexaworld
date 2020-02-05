#include <hexaworld.h>
#include <fstream>
#include <iostream>

using namespace cell_world;
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
    double v = reward * pow(discount,steps) + step_cost * steps;
    //cout << " result " << result << " value " << v << endl;
    return v;
}
