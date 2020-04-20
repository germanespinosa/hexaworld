#pragma once
#include <cell_world.h>
#include <planner/particle_filter.h>
#include <mutex>
#include <utils.h>
#include <atomic>
#include <agents/prey.h>

struct Planner : Prey {
    Planner(cell_world::World &, const cell_world::Cell & , const cell_world::Cell &, double, Reward_config, uint32_t);
    Planner(cell_world::World &, const cell_world::Cell & , const cell_world::Cell &, uint32_t, Reward_config, uint32_t);
    void update(const cell_world::State &) override;
    const cell_world::Cell &start(uint32_t) override;
    void end(Episode_result, uint32_t) override;
    cell_world::Move get_move() override;
    virtual void update_state(uint32_t &) = 0;
    virtual void plan() = 0;
    virtual cell_world::Move get_best_move() = 0;
    Particle_filter set;

protected:
    const cell_world::Cell &_start;
    const cell_world::Cell &goal;
    cell_world::Graph _graph;
    Reward_config _reward_config;
    cell_world::World &_world;

private:
    uint32_t _particle_count;
    void _planning_job();
    double _planning_time;
    int32_t _planning_iterations;
};
