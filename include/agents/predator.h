#pragma once
#include <cell_world.h>
#include <reward_config.h>

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::Graph &, cell_world::Graph &, cell_world::Paths &, const Reward_config &, const cell_world::Cell &);
    const cell_world::Cell &start_episode(unsigned int) override;
    void update_state(const cell_world::State &) override;
    cell_world::Coordinates get_move(const cell_world::State &s) override ;
    void end_episode(const cell_world::State &) override ;
    void set_fixed_start(const cell_world::Cell &);
    void set_random_start();
    static void set_randomness(unsigned int);
private:
    Reward_config _reward_config;
    cell_world::Graph _visibility;
    cell_world::Graph _inverted_visibility;
    bool _fixed_start;
    cell_world::Cell _start;
    cell_world::Cell _last_prey_cell;
    cell_world::Graph _graph;
    cell_world::Map _map;
    cell_world::Move _next_move;
    cell_world::Move _prev_move;
    cell_world::Paths &_paths;
    const cell_world::Cell &_goal;
    friend class Particle_filter;
};
