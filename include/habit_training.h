#pragma once
#include <cellworld.h>
#include "hexaworld.h"

struct Reward_config{
    double success_reward;
    double failure_reward;
    double discount;
    double step_cost;
};

struct Habit{
    Habit( const cell_world::Cell &, const cell_world::Cell &);
    const cell_world::Cell &source;
    const cell_world::Cell &destination;
    cell_world::Cell_group gate_cells;
    cell_world::Connections connections;
    std::vector<double> values;
    std::vector<uint32_t> visits;
    std::vector<uint32_t> policies;
    bool convergence;
    bool load(const std::string&);
    void save(const std::string&);
    void compute_polices();
};

struct Habit_training: cell_world::Agent{
    Habit_training(std::vector<Habit> &, Reward_config, cell_world::Probabilities &);
    const cell_world::Cell & start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Agent_action &get_action() override;
    uint32_t action_ready() override;
    void end_episode(const cell_world::State &) override ;
private:
    uint32_t _current_habit;
    Reward_config _rewards;
    std::vector<Habit> &_habits;
    cell_world::Probabilities & _probabilities;
    Action_set _action_set;
    uint32_t _predator_index;
};
