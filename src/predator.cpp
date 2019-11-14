#include "../include/predator.h"
#include <stdlib.h>

//
// Created by german on 11/13/19.
//

using namespace cellworld;
using namespace std;

Predator::Predator(World &w, Visibility &v, uint32_t prey_id) : world(w), visibility(v), prey_id(prey_id){
    data.id = 0;
    data.color = Green;
    data.status= 1;
    int r;
    while( world[r = rand() % world.size()].occluded);
    data.coordinates = world[r].coordinates;
    action.iteration = 0;
    action.action = {2,0};
}

void Predator::update_state(State state){
    action.iteration = state.iteration;

    bool prey_visible = false;
    Cell &predator_cell  =world[data.coordinates];
    auto conns = world[data.coordinates].connections;
    if (state.agents.size()>prey_id) {
        Cell &prey_cell = world[state.agents[prey_id].coordinates];
        if (visibility.is_visible(predator_cell, prey_cell)){
            action.action = predator_cell.coordinates - data.coordinates;
            double min_dist = world.distance(predator_cell,prey_cell);
            for (unsigned int i=1;i<conns.size();i++){
                double dist = world.distance( world[conns[i]],prey_cell);
                if (dist<min_dist) {
                    action.action = world[conns[i]].coordinates - data.coordinates;
                }
            }
            data.color = Red;
            return ;
        }
    }
    data.color = Green;
    int r = rand() % conns.size();
    action.action = world[conns[r]].coordinates - data.coordinates;
}

bool Predator::get_action(){
    return true;
}

