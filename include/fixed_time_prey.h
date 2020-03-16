#pragma once
#include <cell_world.h>
#include <hexaworld.h>
#include <utils.h>
#include <fixed_time_planner.h>

struct Fixed_time_prey : cell_world::Agent {
    Fixed_time_prey(Fixed_time_planner &, double time, const cell_world::Cell & , const cell_world::Cell &);
    const cell_world::Cell &start_episode() override;
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