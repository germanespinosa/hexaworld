#pragma once
#include <cell_world.h>
#include <hexaworld.h>

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::Graph &);
    const cell_world::Cell &start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    cell_world::Coordinates get_move() override ;
    void end_episode(const cell_world::State &) override ;
    void set_fixed_start(const cell_world::Cell &);
    void set_view_range(double);

private:
    bool _use_view_range;
    double _view_range;
    bool _fixed_start;
    cell_world::Cell _start;
    bool _first_episode;
    bool _chasing;
    cell_world::Cell _last_prey_cell;
    cell_world::Graph _graph;
    cell_world::Move _next_move;
};
