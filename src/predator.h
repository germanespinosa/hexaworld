#ifndef HEXAWORLD_PREDATOR_H
#define HEXAWORLD_PREDATOR_H
#include <cellworld.h>

struct Predator : cellworld::Agent {
    cellworld::World &world;
    cellworld::Visibility &visibility;
    uint32_t *acum;
    Predator(cellworld::World &w, cellworld::Visibility &v);
    void update_state(cellworld::State) override;
    bool get_action() override;
};

#endif //HEXAWORLD_PREDATOR_H
