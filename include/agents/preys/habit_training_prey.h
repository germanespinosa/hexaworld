#pragma once
#include <agents/prey.h>
#include <hexaworld.h>
#include <habit.h>

struct Habit_training_prey: Prey {
    Habit_training_prey(std::vector<Habit> &, Reward_config, double);
    const cell_world::Cell & start(uint32_t) override;
    void update(const cell_world::State &) override;
    cell_world::Coordinates get_move() override;
    void end(Episode_result, uint32_t) override;
    uint32_t episodes;
    uint32_t successes;
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
    std::vector<History> _history;
    std::vector<uint32_t> _sizes;
};
