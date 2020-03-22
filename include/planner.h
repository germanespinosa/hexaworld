#pragma once
#include <cell_world.h>
#include <planner_set.h>
#include <mutex>

struct Planner {
    Planner(const cell_world::World &);
    ~Planner();

    virtual void update_state() = 0;
    virtual void plan() = 0;

    virtual cell_world::Move get_move() = 0;

    Planner_set set;

private:
    void _plan();
    bool _running;
    std::thread _thread;
    std::mutex _mutex;
};
