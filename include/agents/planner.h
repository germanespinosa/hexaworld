#pragma once
#include <cell_world.h>
#include <planner_set.h>
#include <mutex>
#include <utils.h>
#include <atomic>

struct Planner : cell_world::Agent {
    Planner(cell_world::World &, const cell_world::Cell & , const cell_world::Cell &, double, Reward_config);
    ~Planner();
    void update_state(const cell_world::State &) override;
    const cell_world::Cell &start_episode(uint32_t) override;
    void end_episode(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void receive_message(const cell_world::Agent_message &) override;
    virtual void update_state() = 0;
    virtual void plan() = 0;
    virtual cell_world::Move get_best_move() = 0;
    Planner_set set;

protected:
    const cell_world::Cell &_start;
    const cell_world::Cell &_goal;
    cell_world::Graph _graph;
    Reward_config _reward_config;
    cell_world::World &_world;

private:
    void _planning_job();
    std::atomic<bool> _running;
    double _interval;
    std::thread *_thread;
};
