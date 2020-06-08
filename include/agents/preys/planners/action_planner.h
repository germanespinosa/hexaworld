#pragma once

#include <agents/predator.h>
#include <agents/preys/planner_prey.h>
#include <agents/preys/planner.h>

struct Action_planner : Planner {
    Action_planner( cell_world::World &, const cell_world::Cell &, const cell_world::Cell &, Planning_unit, unsigned int, Reward_config, unsigned int, cell_world::Paths &, unsigned int);
    void plan() override ;
    void update_state(unsigned int &) override;
    cell_world::Move get_best_move() override;

    std::vector<cell_world::Move> options;
    std::vector<double> rewards;
    std::vector<unsigned int> visits;
};