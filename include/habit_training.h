#pragma once
#include <cell_world.h>
#include <action_set.h>
#include <hexaworld.h>
#include <habit.h>

struct History{
    uint32_t cell;
    uint32_t action;
};

struct Habit_training: cell_world::Agent{
    Habit_training(std::vector<Habit> &, Reward_config, Action_set, cell_world::Chance &, uint32_t , uint32_t );
    const cell_world::Cell & start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Agent_action &get_action() override;
    void end_episode(const cell_world::State &) override ;
    void set_fixed_start(const cell_world::Cell &);
    uint32_t episodes;
    uint32_t success;
private:
    void _add_history(uint32_t, uint32_t);
    bool _fixed_start;
    cell_world::Cell _start;
    std::vector<History> _history;
    uint32_t _iteration;
    uint32_t _current_habit;
    Reward_config _rewards;
    std::vector<Habit> &_habits;
    cell_world::Chance & _probabilities;
    Action_set _action_set;
    Episode_result  _episode_result;
    cell_world::Chance _habit_probability;
};
