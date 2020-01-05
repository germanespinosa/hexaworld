#ifndef HEXAWORLD_PREDATOR_H
#define HEXAWORLD_PREDATOR_H
#include <cellworld.h>
#define PREDATOR_TYPE 2

struct Predator : cellworld::Agent {
    // called from the simulation
    void start_episode(const cellworld::State &) override;
    void update_state(const cellworld::State &) override;
    void end_episode(const cellworld::State &) override;
    uint32_t action_ready() override;
    uint32_t get_destination() override;

    Predator(cellworld::World &, cellworld::Visibility &);
    void reset();
private:
    const std::vector<cellworld::Coordinates> _predator_moves {{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}};
    cellworld::World &_world;
    cellworld::Visibility &_visibility;
    uint32_t _prey_id;
    uint32_t _iteration;
    uint32_t _next_action;
    cellworld::Connections _wc;
};

#endif //HEXAWORLD_PREDATOR_H
