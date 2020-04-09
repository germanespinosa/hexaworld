#pragma once
#include <cell_world.h>
#include <reward_config.h>

struct Prey : cell_world::Agent {
    Prey();
    const cell_world::Cell &start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    void end_episode(const cell_world::State &) override;
    void receive_message(const cell_world::Agent_message &) override;

    virtual const cell_world::Cell &start(uint32_t) = 0;
    virtual void update(const cell_world::State &);

    virtual void end(Episode_result, uint32_t);

    void set_goal(const cell_world::Cell &);

private:
    bool _eaten;
    bool _ready;
    uint32_t _start_iteration;
    cell_world::Cell _goal;
};