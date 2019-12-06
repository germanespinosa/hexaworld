#include "../include/prey.h"
#include <stdlib.h>

//
// Created by german on 11/13/19.
//

using namespace cellworld;
using namespace std;

Prey::Prey(World &w, Visibility &v, uint32_t predator_id) : world(w), visibility(v), predator_id(predator_id){
    data.id = 0;
    data.color = Green;
    data.status= 1;
    int r;
    while( world[r = rand() % world.size()].occluded);
    data.coordinates = world[r].coordinates;
    action.iteration = 0;
    action.action = {2,0};
    active = true;
}

void Prey::update_state(State state){
    action.iteration = state.iteration;
    if (!active) return;
    reward--;
    bool prey_visible = false;
    Cell &predator_cell = world[data.coordinates];
    auto conns = world[data.coordinates].connections;
    if (state.agents.size()>predator_id) {
        if (state.agents[predator_id].coordinates == data.coordinates) {
            reward -=1000;
            active = false;
        } else if (Coordinates({20,0}) == data.coordinates) {
            reward +=1000;
            active = false;
        }
    }
    data.color = Green;
    int r = rand() % conns.size();
    action.action = world[conns[r]].coordinates - data.coordinates;
}

bool Prey::get_action(){
    return true;
}

Prey::~Prey(){
    //save state
}

