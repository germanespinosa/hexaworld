#include <partial_vision.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cellworld;

std::vector<uint32_t> Partial_vision::get_options(uint32_t cell_id) {
    return _wc.cell_connections(cell_id);
}

Prey_expected_reward *Partial_vision::get_expected_rewards(Prey_state_action &sa) {
    if (sa.last_seen == -1 || sa.iteration - sa.last_seen >= _memory) {
        return _habit[sa.prey_cell_id * _prey_moves.size()];
    } else{
        return _buffer[(sa.iteration - sa.last_seen) * _world.size() * _world.size() * _prey_moves.size() +
        sa.predator_cell_id * _world.size() * _prey_moves.size() +
        sa.prey_cell_id * _prey_moves.size()];
    }
}

void Partial_vision::save_expected_reward(Prey_state_action &sa, double reward, uint32_t episode_steps) {
    double step_reward = reward * ( pow(_config.discount,episode_steps - sa.iteration) ) + _config.step_cost * sa.iteration;
    if (sa.last_seen == -1 || sa.iteration - sa.last_seen >= _memory) {
        uint32_t index = sa.prey_cell_id * _prey_moves.size() + sa.action;
        double aging = ((double)_habit[index]->visits) / ((double)_habit[index]->visits + 1);
        _habit[index]->reward = aging * _habit[index]->reward + step_reward * ( 1 - aging);
        _habit[index]->visits ++;
    } else {
        if (sa.iteration == sa.last_seen) _predator_contacts++;
        uint32_t index = (sa.iteration - sa.last_seen) * _world.size() * _world.size() * _prey_moves.size() +
                         sa.predator_cell_id * _world.size() * _prey_moves.size() +
                         sa.prey_cell_id * _prey_moves.size() +
                         sa.action;
        double aging = ((double)_buffer[index]->visits) / ((double)_buffer[index]->visits + 1);
        _buffer[index]->reward = aging * _buffer[index]->reward + step_reward * ( 1 - aging);
        _buffer[index]->visits ++;
    }

}

Partial_vision::Partial_vision(World &w, Visibility &vi, Prey_config &config) :
        _buffer(_memory * w.size() * w.size() * _prey_moves.size()),
        _habit(w.size() * _prey_moves.size()),
        _wc (w, _prey_moves),
        Prey(w,vi,config)
{
    _predator_contacts = 0;

    if (!_buffer.load("partial_vision.dat")) {
    }
    if (!_habit.load("partial_vision_habit.dat")) {
    }
}

Partial_vision::~Partial_vision() {
    _buffer.save("partial_vision.dat");
    _habit.save("partial_vision_habit.dat");
    cout << "Predator contacts: " << _predator_contacts << endl;
}
