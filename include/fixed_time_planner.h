#pragma once
#include <planner_history.h>
#include <cell_world.h>

struct Fixed_time_planner {
    Fixed_time_planner();
    virtual void plan(uint32_t iteration, Planner_history &prey_history, Planner_history &predator_history);
    bool running{};
    cell_world::Move best_move{};
private:
    void _plan();
    void start_planning();
    cell_world::Move get_move();
    Planner_history _predator_history;
    Planner_history _prey_history;
    std::thread *_thread{};
    friend class Fixed_time_prey;
};
