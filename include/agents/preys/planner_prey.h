#pragma once

#include <agents/prey.h>
#include <reward_config.h>
#include <planner_history.h>
#include <hexaworld.h>

struct Planner_prey: Prey {
    explicit Planner_prey(const cell_world::Cell &);
    const cell_world::Cell &start(uint32_t) override;
    void update(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void end(Episode_result, uint32_t) override;

    void set_move(const cell_world::Move &);
    void set_start_cell(const cell_world::Cell &);

    cell_world::Cell goal;
    bool contact;
    uint32_t current_move;
    cell_world::Cell start_cell;
    cell_world::Move next_move;

    Episode_result result;
    uint32_t lenght;
};