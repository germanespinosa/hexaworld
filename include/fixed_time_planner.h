#pragma once
#include <planner_history.h>
#include <cell_world.h>

struct Fixed_time_planner {
    Fixed_time_planner();
    virtual void plan(uint32_t iteration, Planner_history &prey_history, Planner_history &predator_history);
    cell_world::Move get_move();
    bool action_ready{};
    bool running{};
    cell_world::Move best_move{};
    void start_planning();
    Planner_history predator_history;
    Planner_history prey_history;
private:
    void _plan();
    virtual cell_world::Move _get_best_move();
    std::thread *_thread{};
};
