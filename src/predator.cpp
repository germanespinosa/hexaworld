#include "predator.h"
#include <stdlib.h>

//
// Created by german on 11/13/19.
//

using namespace cellworld;
using namespace std;

Predator::Predator(World &w, Visibility &v) : world(w), visibility(v){
    data.id = 1;
    data.color = Green;
    data.status=0;
    int r;
    while( world[r = rand() % world.size()].occluded);
    data.coordinates = world[r].coordinates;
    action.iteration = 0;
    action.action = {2,0};
    acum = (uint32_t *)malloc (world.size() * sizeof(uint32_t));
    for (unsigned int i=0;i<world.size();i++) acum[i] = 0;
}

void Predator::update_state(State state){
    action.iteration = state.iteration;
    acum[world[data.coordinates].id]++;
    world[data.coordinates].value++;
}

bool Predator::get_action(){
    auto conns = world[data.coordinates].connections;
    int r = rand() % conns.size();
    action.action = world[conns[r]].coordinates - data.coordinates;
    return true;
}

