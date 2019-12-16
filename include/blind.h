//
// Created by german on 12/8/19.
//

#ifndef HEXAWORLD_BLIND_H
#define HEXAWORLD_BLIND_H
#include "prey.h"

struct Blind : Prey{
    Blind(cellworld::World &, cellworld::Visibility &, Prey_config &);
    std::vector<uint32_t> get_options(uint32_t) override ;
    Prey_expected_reward *get_expected_rewards(Prey_state_action &) override ;
    void save_expected_reward (Prey_state_action &, double, uint32_t) override ;
    ~Blind();
private:
    const std::vector<cellworld::Coordinates> _prey_moves {{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}};
    Prey_expected_reward_buffer _buffer;
    cellworld::World_connections _wc;
};

#endif //HEXAWORLD_BLIND_H
