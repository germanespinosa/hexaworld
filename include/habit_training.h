#pragma once
#include <cell_world.h>
#include <hexaworld.h>
#include <habit.h>

struct Habit_training: cell_world::Agent{
    Habit_training(std::vector<Habit> &, Reward_config, double);
    const cell_world::Cell & start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    cell_world::Coordinates get_move() override;
    void end_episode(const cell_world::State &) override ;
    uint32_t episodes;
    uint32_t success;
private:
    struct History{
        uint32_t cell_index;
        uint32_t action_index;
    };
    cell_world::Move _next_move;
    uint32_t _start_cell_index;
    uint32_t _start_action_index;
    uint32_t _iteration;
    uint32_t _current_habit;
    Reward_config _rewards;
    std::vector<Habit> &_habits;
    Episode_result _episode_result;
    cell_world::Chance _habit_probability;
    std::vector<History> _history;
};
