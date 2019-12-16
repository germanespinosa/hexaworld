#include <blind.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cellworld;

std::vector<uint32_t> Blind::get_options(uint32_t cell_id) {
    return _wc.cell_connections(cell_id);
}

Prey_expected_reward *Blind::get_expected_rewards(Prey_state_action &sa) {
    return _buffer[sa.prey_cell_id * _prey_moves.size()];
}

void Blind::save_expected_reward(Prey_state_action &sa, double reward, uint32_t episode_steps) {
    uint32_t index = sa.prey_cell_id * _prey_moves.size() + sa.action;
    double aging = ((double)_buffer[index]->visits) / ((double)_buffer[index]->visits + 1);
    double step_reward = reward * ( pow(_config.discount,episode_steps - sa.iteration) ) + _config.step_cost * sa.iteration;
    _buffer[index]->reward = aging * _buffer[index]->reward + step_reward * ( 1 - aging);
    (double)_buffer[index]->visits ++;
}

Blind::Blind(World &w, Visibility &vi, Prey_config &config) :
    _buffer(w.size() * _prey_moves.size()),
    _wc (w, _prey_moves),
    Prey(w,vi,config)
{
    if (!_buffer.load("blind.dat")) {/*
        vector<double> v(_world.size());
        for (unsigned int i = 0; i < _world.size(); i++) v[i] = _world.size();
        v[config.goal] = 1;
        bool changes = true;
        while (changes) {
            changes = false;
            for (unsigned int i = 0; i < _world.size(); i++) {
                if (!_world[i].occluded) {
                    auto c = _wc.cell_connections(i);
                    for (unsigned int j = 0; j < c.size(); j++) {
                        if (v[c[j]] + 1 < v[i]) {
                            v[i] = v[c[j]] + 1;
                            changes = true;
                        }
                    }
                }
            }
        }
        for (unsigned int i = 0; i < _world.size(); i++) {
            _world.set_value(i, v[i]);
            if (!_world[i].occluded) {
                auto c = _wc.cell_connections(i);
                for (unsigned int j = 0; j < c.size(); j++) {
                    _buffer[i * _prey_moves.size() + j]->reward = -v[c[j]];
                    _buffer[i * _prey_moves.size() + j]->visits = 1;
                }
            }
        }*/
    }
}

Blind::~Blind() {
    _buffer.save("blind.dat");
}
