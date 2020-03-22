#pragma once
#include <cell_world.h>
#include <hexaworld.h>
#include <utils.h>
#include <planner.h>

struct Fixed_time_prey : cell_world::Agent {
    Fixed_time_prey(uint32_t, Planner &, double, const cell_world::Cell & , const cell_world::Cell &);
    const cell_world::Cell &start_episode() override;
    void update_state(const cell_world::State &) override;
    cell_world::Move get_move() override;
    void end_episode(const cell_world::State &) override;
private:
    uint32_t _steps;
    Planner &_planner;
    Stop_watch _stop_watch;
    double _time_out;
    const cell_world::Cell &_start;
    const cell_world::Cell &_goal;
};