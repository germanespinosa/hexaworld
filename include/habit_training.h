#pragma once
#include <cellworld.h>
#include <action_set.h>
#include <hexaworld.h>
#include <habit.h>

struct History{
    uint32_t cell;
    uint32_t action;
};

struct Habit_training: cell_world::Agent{
    Habit_training(std::vector<Habit> &, Reward_config, Action_set, cell_world::Probabilities &);
    const cell_world::Cell & start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Agent_action &get_action() override;
    uint32_t action_ready() override;
    void end_episode(const cell_world::State &) override ;
private:
    void _add_history(uint32_t, uint32_t);
    std::vector<History> _history;
    uint32_t _iteration;
    uint32_t _current_habit;
    Reward_config _rewards;
    std::vector<Habit> &_habits;
    cell_world::Probabilities & _probabilities;
    Action_set _action_set;
    uint32_t _predator_index;
    double _episode_reward;
    cell_world::Probabilities _habit_probability;
};
