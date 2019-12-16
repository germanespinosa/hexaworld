#include <full_vision.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cellworld;

std::vector<uint32_t> Full_vision::get_options(uint32_t cell_id) {
    return _wc.cell_connections(cell_id);
}

Prey_expected_reward *Full_vision::get_expected_rewards(Prey_state_action &sa) {
    return _buffer[sa.current_predator_cell_id * _world.size() * _prey_moves.size()+ sa.prey_cell_id * _prey_moves.size()];
}

void Full_vision::save_expected_reward(Prey_state_action &sa, double reward, uint32_t episode_steps) {
    uint32_t index = sa.current_predator_cell_id * _world.size() * _prey_moves.size() + sa.prey_cell_id * _prey_moves.size() + sa.action;
    double aging = ((double)_buffer[index]->visits) / ((double)_buffer[index]->visits + 1);
    double step_reward = reward * ( pow(_config.discount,episode_steps - sa.iteration) ) + _config.step_cost * sa.iteration;
    _buffer[index]->reward = aging * _buffer[index]->reward + step_reward * ( 1 - aging);
    (double)_buffer[index]->visits ++;
}

Full_vision::Full_vision(World &w, Visibility &vi, Prey_config &config) :
        _buffer(w.size() * w.size() * _prey_moves.size()),
        _wc (w, _prey_moves),
        Prey(w,vi,config)
{
    if (!_buffer.load("full_vision.dat")) {
    }
}

Full_vision::~Full_vision() {
    _buffer.save("full_vision.dat");
}
