#include <blind.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cell_world;

Prey_expected_reward *Blind::get_expected_rewards(Prey_state_action &sa) {
    return _buffer[sa.prey_cell_id * _prey_moves.size()];
}

void Blind::save_expected_reward(Prey_state_action &sa, double reward, uint32_t episode_steps) {
    uint32_t index = sa.prey_cell_id * _prey_moves.size() + sa.action;
    double aging = ((double)_buffer[index]->visits) / ((double)_buffer[index]->visits + 1);
    double step_reward = reward * ( pow(_config.discount,episode_steps - sa.iteration) ) + _config.step_cost * sa.iteration;
    _buffer[index]->reward = aging * _buffer[index]->reward + step_reward * ( 1 - aging);
    _buffer[index]->visits ++;
}

Blind::Blind(World &w, Visibility &vi, Prey_config &config) :
    _buffer(w.size() * _prey_moves.size()),
    Prey(w,vi,config)
{
    w.get_connections(_wc,CONTACT_CELLS);
    if (!_buffer.load("blind.dat")) {
    }
}

Blind::~Blind() {
    _buffer.save("blind.dat");
}

const std::vector<uint32_t> &Blind::get_options(uint32_t cell_id) {
    return _wc[cell_id].get_all();
}
