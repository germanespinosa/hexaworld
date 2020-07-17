#pragma once
#include <cell_world.h>
#include <reward_config.h>

struct Prey : cell_world::Agent {
    Prey();
    const cell_world::Cell &start_episode(unsigned int) override;
    void update_state(const cell_world::State &) override;
    void end_episode(const cell_world::State &) override;
    cell_world::Move get_move(const cell_world::State &) override;

    virtual const cell_world::Cell &start(unsigned int) = 0;
    virtual void update(const cell_world::State &);

    virtual void end(Episode_result, unsigned int);
    virtual cell_world::Move get_move() = 0;
    void set_goal(const cell_world::Cell &);
    unsigned int lenght;
protected:
    Episode_result _result;

private:
    bool _ready;
    unsigned int _start_iteration;
    cell_world::Cell _goal;
};
