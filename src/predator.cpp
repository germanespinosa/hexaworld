#include "../include/predator.h"
#include <stdlib.h>
#include <iostream>
#include "prey.h"
//
// Created by german on 11/13/19.
//

using namespace cellworld;
using namespace std;

void Predator::reset() {
    int r;
    while (_world[r=rand() % _world.size()].occluded);
    data.cell_id = r;
    _next_action = data.cell_id;
    _iteration = 0;
    data.status = 1;
}

Predator::Predator(World &w, Visibility &v)
    : _world(w)
    , _visibility(v)
{
    w.get_connections(_wc,CONTACT_CELLS);
    data.type = PREDATOR_TYPE;
    data.id = 0;
    data.color = Yellow;
}

void Predator::update_state(const State &state){
    _iteration = state.iteration;
    auto conns = _wc[data.cell_id];
    if (state.agents[_prey_id]->status == 0) {
        active = false;
        return;
    }
    uint32_t prey_cell_id = state.agents[_prey_id]->cell_id;
    if (state.agents[_prey_id]->status == 0) active = 0;
    if (data.cell_id == prey_cell_id){
        active = false;
        _next_action = data.cell_id;
    }
    if (_visibility.is_visible(state.agents[_prey_id]->cell_id, data.cell_id)){
        double min_dist = _world.distance(conns[0],prey_cell_id);
        _next_action = data.id;
        for (unsigned int i=1;i<conns.size();i++){
            double dist = _world.distance(conns[i],prey_cell_id);
            if ( dist < min_dist) {
                _next_action = conns[i];
                min_dist = dist;
            }
        }
        if (_next_action == prey_cell_id) data.status = 2;// got the prey
        data.color = Red;
    }else {
        data.color = Yellow;
        int r = rand() % conns.size();
        _next_action = conns[r];
    }
}

uint32_t Predator::action_ready() {
    return _iteration;
}

uint32_t Predator::get_destination() {
    return _next_action;
}

void Predator::start_episode(const cellworld::State &state) {
    reset();
    for (unsigned int i=0 ; i<state.agents.size(); i++)
        if (state.agents[i]->type == PREY_TYPE) _prey_id = i;
}

void Predator::end_episode(const cellworld::State &) {
}

