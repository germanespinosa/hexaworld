#pragma once

#include <reward_config.h>
#include <cell_world.h>
#include <planner_history.h>
#include <hexaworld.h>

struct Planner_prey: cell_world::Agent{
    explicit Planner_prey(cell_world::Graph &);
    const cell_world::Cell &start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    void end_episode(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void set_move(const cell_world::Move &);

    void set_start_cell(const cell_world::Cell &);
    void pick_random_move();
    bool filtered;

    std::vector<cell_world::Move> trajectory;
    uint32_t current_move;

    cell_world::Cell start_cell;

    cell_world::Cell goal;
    cell_world::Graph graph;
    cell_world::Move next_move;

    Reward_config reward_config;
    uint32_t start_iteration;
    uint32_t unknown;
    uint32_t successes;
    uint32_t fails;

    double reward;
};