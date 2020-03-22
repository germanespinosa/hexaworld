#pragma once

#include <fixed_time_planner.h>
#include <predator.h>
#include <planner_prey.h>
#include <planner.h>

struct Action_planner : Planner {
    explicit Action_planner(const cell_world::World &, const cell_world::Cell &);
    void plan() override ;
    void update_state() override;
    cell_world::Move get_move() override;
    const cell_world::Cell &goal;
    std::vector<cell_world::Move> options;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
};