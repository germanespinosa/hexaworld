#pragma once

#include <agents/predator.h>
#include <agents/preys/planner_prey.h>
#include <agents/preys/planner.h>

struct Action_planner : Planner {
    Action_planner( cell_world::World &, const cell_world::Cell &, const cell_world::Cell &, Planning_unit, uint32_t, Reward_config, uint32_t, cell_world::Paths &, uint32_t);
    void plan() override ;
    void update_state(uint32_t &) override;
    cell_world::Move get_best_move() override;

    std::vector<cell_world::Move> options;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
};