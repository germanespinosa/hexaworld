#pragma once
#include <agents/prey.h>
#include <habits/simulation.h>

struct Habit_training_prey: Prey {
    Habit_training_prey(std::vector<Habit> &, Reward_config, double);
    const cell_world::Cell & start(unsigned int) override;
    void update(const cell_world::State &) override;
    cell_world::Coordinates get_move() override;
    void end(Episode_result, unsigned int) override;
    unsigned int episodes;
    unsigned int successes;
private:
    struct History{
        unsigned int cell_index;
        unsigned int action_index;
    };
    cell_world::Move _next_move;
    unsigned int _start_cell_index;
    unsigned int _start_action_index;
    unsigned int _current_habit;
    Reward_config _rewards;
    std::vector<Habit> &_habits;
    std::vector<History> _history;
    std::vector<unsigned int> _sizes;
};
