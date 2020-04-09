#pragma once

#include <agents/predator.h>
#include <agents/preys/planner_prey.h>
#include <agents/preys/planner.h>

struct Action_planner : Planner {
    explicit Action_planner( cell_world::World &, const cell_world::Cell &, const cell_world::Cell &, double, Reward_config);
    void plan() override ;
    void update_state() override;
    cell_world::Move get_best_move() override;

    std::vector<cell_world::Move> options;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
    std::vector<uint32_t> successes;
    std::vector<uint32_t> unknowns;
    std::vector<uint32_t> fails;
};