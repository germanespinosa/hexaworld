#pragma once
#include <cell_world.h>
#include <hexaworld.h>

struct Predator_actions{
    Predator_actions(cell_world::Cell_group &,cell_world::Connection_pattern, cell_world::Probabilities &);
    cell_world::Agent_action &operator[](uint32_t);
    uint32_t  get_best (cell_world::Location);
private:
    std::vector<cell_world::Agent_action> _actions;
    std::vector<cell_world::Location> _displacements;
    void _add_action(cell_world::Location, cell_world::Cell_group &,  cell_world::Probabilities &);
};

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::Cell_group, cell_world::Visibility &) ;
    const cell_world::Cell &start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    uint32_t action_ready() override;
    cell_world::Agent_action &get_action() override ;
    void end_episode(const cell_world::State &) override ;
private:
    bool _chasing;
    cell_world::Cell _last_prey_cell;
    cell_world::Cell_group _cg;
    int32_t _next_action;
    int32_t _prey_index;
    int32_t _iteration;
    cell_world::Agent_action _random_action;
    cell_world::Probabilities _probabilities;
    Predator_actions _actions;
    cell_world::Visibility &_visibility;
};

struct Test_prey : cell_world::Agent {
    Test_prey(cell_world::Cell_group);
    const cell_world::Cell &start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    uint32_t action_ready() override;
    cell_world::Agent_action &get_action() override;
    void end_episode(const cell_world::State &) override;
private:
    cell_world::Agent_action _random_action;
    cell_world::Cell_group _cg;
    uint32_t _iteration;
};