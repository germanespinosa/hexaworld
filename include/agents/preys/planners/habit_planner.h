#pragma once

#include <agents/predator.h>
#include <agents/preys/planner_prey.h>
#include <agents/preys/planner.h>
#include <habits/habit.h>

struct Habit_planner : Planner {
    explicit Habit_planner( cell_world::World &, const cell_world::Cell_group &, const cell_world::Cell &,
                           const cell_world::Cell &, double , Reward_config);
    void plan() override ;
    void update_state() override;
    cell_world::Move get_best_move() override;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
    Habit_set _habit_set;
    std::vector<std::reference_wrapper<Habit>> options;
};