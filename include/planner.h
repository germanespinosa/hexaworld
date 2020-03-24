#pragma once
#include <cell_world.h>
#include <planner_set.h>
#include <mutex>
#include <utils.h>

struct Planner : cell_world::Agent {
    Planner(const cell_world::World &, const cell_world::Cell & , const cell_world::Cell &, double);
    ~Planner();

    void update_state(const cell_world::State &) override;
    const cell_world::Cell &start_episode(uint32_t) override;
    void end_episode(const cell_world::State &) override;
    virtual void update_state() = 0;
    virtual void plan() = 0;
    Planner_set set;

protected:
    const cell_world::Cell &_start;
    const cell_world::Cell &_goal;
    Stop_watch _stop_watch;
    Stop_watch _clock;
    uint32_t _iterations;

private:
    void _planning_job();
    bool _running = true;
    double _interval;
    //std::thread _thread;
    std::mutex _mutex;
};
