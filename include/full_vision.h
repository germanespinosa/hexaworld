//
// Created by german on 12/15/19.
//

#ifndef HEXAWORLD_FULL_VISION_H
#define HEXAWORLD_FULL_VISION_H
#include "prey.h"

struct Full_vision : Prey{
    Full_vision(cellworld::World &, cellworld::Visibility &, Prey_config &);
    const std::vector<uint32_t> & get_options(uint32_t) override;
    Prey_expected_reward *get_expected_rewards(Prey_state_action &) override ;
    void save_expected_reward (Prey_state_action &, double, uint32_t) override ;
    ~Full_vision();
private:
    const std::vector<cellworld::Coordinates> _prey_moves {{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}};
    Prey_expected_reward_buffer _buffer;
    cellworld::Connections _wc;
};

#endif //HEXAWORLD_FULL_VISION_H
