#pragma once
#include <cell_world.h>

struct Predator : cell_world::Agent {
    explicit Predator(cell_world::Graph &);
    const cell_world::Cell &start_episode(uint32_t) override;
    void update_state(const cell_world::State &) override;
    cell_world::Coordinates get_move() override ;
    void end_episode(const cell_world::State &) override ;
    void set_fixed_start(const cell_world::Cell &);
    void set_random_start();
    void track_history();
    bool _fixed_start;
    std::vector<cell_world::Coordinates> history;
private:
    bool _track_history;
    cell_world::Cell _start;
    bool _chasing;
    bool _contact;
    cell_world::Cell _last_prey_cell;
    cell_world::Graph _graph;
    cell_world::Move _next_move;
};
