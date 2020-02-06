#pragma once
#include <cell_world.h>
#include <action_set.h>
#include "hexaworld.h"

struct Habit_value{
    Habit_value(uint32_t);
    std::vector<double> rewards;
    std::vector<uint32_t> visits;
    double length;
    uint32_t fails;
    uint32_t successes;
    uint32_t unknowns;
    uint32_t get_policy();
    uint32_t get_visits();
};

struct Habit_policies{
    uint32_t sub_world_id;
    cell_world::Cell source;
    cell_world::Cell destination;
    double lenght;
    double death_rate;
    cell_world::Cell_group gate_cells;
    std::vector<double> probabilities;
    cell_world::Cell_group cells;
    std::vector<uint32_t> policies;
};

struct Habit{
    uint32_t sub_world_id;
    cell_world::Cell destination;
    cell_world::Cell_group cells;
    cell_world::Cell_group gate_cells;
    std::vector<Habit_value> values;
    void clear_stats();
    void init_values(uint32_t);
    bool convergence;
    bool load(const std::string&, uint32_t);
    bool save(const std::string&);
    void add_reward(uint32_t, uint32_t,Reward_config &, Episode_result, uint32_t);
private:
    const std::string _extension = ".habit";
};
