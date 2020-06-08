#pragma once

#include <agents/prey.h>
#include <reward_config.h>

struct Planner_prey: Prey {
    explicit Planner_prey(const cell_world::Cell &);
    const cell_world::Cell &start(unsigned int) override;
    void update(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void end(Episode_result, unsigned int) override;

    void set_move(const cell_world::Move &);
    void set_start_cell(const cell_world::Cell &);

    cell_world::Cell goal;
    bool contact;
    cell_world::Cell start_cell;
    cell_world::Move next_move;

    Episode_result result;
    friend class Particle_filter;
};