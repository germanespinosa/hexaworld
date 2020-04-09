#pragma once
#include <agents/prey.h>
#include <hexaworld.h>

struct Test_prey : Prey {
    explicit Test_prey(cell_world::Graph &);
    const cell_world::Cell &start(uint32_t) override;
    void update(const cell_world::State &) override;
    cell_world::Move get_move() override;
private:
    cell_world::Graph &_cg;
    bool _clean;
};