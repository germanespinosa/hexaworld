#pragma once
#include <cell_world.h>
#include <hexaworld.h>
#include <action_set.h>

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::World &, const cell_world::Chance&);
    const cell_world::Cell &start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Agent_action &get_action() override ;
    void end_episode(const cell_world::State &) override ;
    void set_fixed_start(const cell_world::Cell &);
    void set_view_range(double);

private:
    std::vector<uint32_t> _visits;
    bool _use_view_range;
    double _view_range;
    bool _fixed_start;
    cell_world::Cell _start;
    bool _first_episode;
    bool _chasing;
    cell_world::Cell _last_prey_cell;
    cell_world::Cell_group _cg;
    int32_t _next_action;
    cell_world::Agent_action _random_action;
    cell_world::Chance _probabilities;
    Action_set _actions;
};

struct Test_prey : cell_world::Agent {
    Test_prey(cell_world::Cell_group);
    const cell_world::Cell &start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    cell_world::Agent_action &get_action() override;
    void end_episode(const cell_world::State &) override;
private:
    cell_world::Agent_action _random_action;
    cell_world::Cell_group _cg;
};