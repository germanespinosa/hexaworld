#ifndef HEXAWORLD_PREDATOR_H
#define HEXAWORLD_PREDATOR_H
#include <cellworld.h>

struct Predator : cellworld::Agent {
    cellworld::World &world;
    cellworld::Visibility &visibility;
    uint32_t prey_id;
    Predator(cellworld::World &, cellworld::Visibility &, uint32_t);
    void update_state(cellworld::State) override;
    bool get_action() override;
};

#endif //HEXAWORLD_PREDATOR_H
