#pragma once
#include <cell_world.h>
#include <hexaworld.h>
#include <utils.h>

struct Fixed_time_planner {
    virtual void plan();
    void start_planning(const cell_world::State &);
    cell_world::Move get_move();

    bool running;
    cell_world::Move move;
    cell_world::State state;
private:
    std::thread *_thread;
};

struct Fixed_time_prey : cell_world::Agent {
    Fixed_time_prey(Fixed_time_planner &, double time, const cell_world::Cell & , const cell_world::Cell &);
    const cell_world::Cell &start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void end_episode(const cell_world::State &) override;
private:
    Fixed_time_planner &_planner;
    Stop_watch _stop_watch;
    double _time_out;
    const cell_world::Cell &_start;
    const cell_world::Cell &_goal;
};