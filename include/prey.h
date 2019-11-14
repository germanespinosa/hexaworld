#ifndef HEXAWORLD_PREY_H
#define HEXAWORLD_PREY_H
#include <cellworld.h>

struct Prey : cellworld::Agent {
    cellworld::World &world;
    cellworld::Visibility &visibility;
    uint32_t predator_id;
    Prey(cellworld::World &, cellworld::Visibility &, uint32_t);
    void update_state(cellworld::State) override;
    bool get_action() override;
};

#endif //HEXAWORLD_PREY_H
