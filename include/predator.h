#ifndef HEXAWORLD_PREDATOR_H
#define HEXAWORLD_PREDATOR_H
#include <cellworld.h>
#define PREDATOR_TYPE 2

struct Predator : cell_world::Agent {
    // called from the simulation
    void start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    void end_episode(const cell_world::State &) override;
    uint32_t action_ready() override;
    uint32_t get_destination() override;

    Predator(cell_world::World &, cell_world::Visibility &);
    void reset();
private:
    const std::vector<cell_world::Coordinates> _predator_moves {{0,0},{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}};
    cell_world::World &_world;
    cell_world::Visibility &_visibility;
    uint32_t _prey_id;
    uint32_t _iteration;
    uint32_t _next_action;
    cell_world::Connections _wc;
};

#endif //HEXAWORLD_PREDATOR_H
