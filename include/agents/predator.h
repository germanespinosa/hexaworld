#pragma once
#include <cell_world.h>

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::Graph &, cell_world::Graph &);
    const cell_world::Cell &start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    cell_world::Coordinates get_move() override ;
    void end_episode(const cell_world::State &, const cell_world::History &) override ;
    void set_fixed_start(const cell_world::Cell &);
    void set_random_start();
    static void set_randomness(uint32_t);
    bool _contact;
private:
    cell_world::Graph _inverted_visibility;
    bool _fixed_start;
    cell_world::Cell _start;
    bool _chasing;
    cell_world::Cell _last_prey_cell;
    cell_world::Graph _graph;
    cell_world::Move _next_move;
    cell_world::Move _prev_move;
    friend class Particle_filter;
};
