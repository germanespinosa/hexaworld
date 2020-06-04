#pragma once

#include <agents/predator.h>
#include <agents/preys/planner_prey.h>
#include <agents/preys/planner.h>
#include <habits/habit.h>

enum class Planning_strategy{
    micro_habits,
    shortest_path
};

struct Point_planner : Planner {
    Point_planner(cell_world::World &, const cell_world::Cell_group &, const cell_world::Cell &,
                  const cell_world::Cell &, Planning_strategy, Planning_unit, uint32_t , Reward_config, uint32_t, cell_world::Paths &, uint32_t);
    void plan() override ;
    void update_state(uint32_t &) override;
    cell_world::Move get_best_move() override;
    Planning_strategy planning_strategy;
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
    Habit_set _habit_set;
    std::vector<std::reference_wrapper<Habit>> options;
    int32_t _last_destination;
};