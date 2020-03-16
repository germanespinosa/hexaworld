#pragma once
#include <cell_world.h>
#include <hexaworld.h>

struct Test_prey : cell_world::Agent {
    Test_prey(cell_world::Graph &);
    const cell_world::Cell &start_episode() override;
    void update_state(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void end_episode(const cell_world::State &) override;
private:
    cell_world::Graph &_cg;
    bool _clean;
};