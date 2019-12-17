//
// Created by german on 12/16/19.
//

#ifndef HEXAWORLD_PARTIAL_VISION_H
#define HEXAWORLD_PARTIAL_VISION_H
#include "prey.h"

struct Partial_vision : Prey{
    Partial_vision(cellworld::World &, cellworld::Visibility &, Prey_config &);
    std::vector<uint32_t> get_options(uint32_t) override ;
    Prey_expected_reward *get_expected_rewards(Prey_state_action &) override ;
    void save_expected_reward (Prey_state_action &, double, uint32_t) override ;
    ~Partial_vision();
private:
    const int _memory = 10;
    const std::vector<cellworld::Coordinates> _prey_moves {{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}};
    Prey_expected_reward_buffer _buffer;
    Prey_expected_reward_buffer _habit;
    cellworld::World_connections _wc;
    uint32_t _predator_contacts;
};

#endif //HEXAWORLD_PARTIAL_VISION_H
