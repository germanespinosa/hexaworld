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
                  const cell_world::Cell &, Planning_strategy, Planning_unit, unsigned int , Reward_config, unsigned int, cell_world::Paths &, unsigned int);
    void plan() override ;
    void update_state(unsigned int &) override;
    cell_world::Move get_best_move() override;
    Planning_strategy planning_strategy;
    std::vector<double> rewards;
    std::vector<unsigned int> visits;
    Habit_set _habit_set;
    std::vector<std::reference_wrapper<Habit>> options;
    int _last_destination;
};