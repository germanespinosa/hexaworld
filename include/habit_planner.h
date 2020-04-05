#pragma once

#include <fixed_time_planner.h>
#include <predator.h>
#include <planner_prey.h>
#include <planner.h>
#include <habit.h>

struct Habit_planner : Planner {
    explicit Habit_planner(const cell_world::World &, const cell_world::Cell_group &, const cell_world::Cell &,
                           const cell_world::Cell &, double , Reward_config);
    void plan() override ;
    void update_state() override;
    cell_world::Move get_best_move() override;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
    std::vector<uint32_t> successes;
    std::vector<uint32_t> unknowns;
    std::vector<uint32_t> fails;
    Habit_set _habit_set;
    std::vector<std::reference_wrapper<Habit>> options;
};