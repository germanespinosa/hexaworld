#include <iostream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"

#define ADJACENT_CELLS cell_world::Connection_pattern({{-2,0},{-1,-1},{1,-1},{2,0},{1,1},{-1,1}})
using namespace ge211;
using namespace std;
using namespace cell_world;

Location get_location(Coordinates c) {
    return {(double)c.x *  .5 ,  (double)c.y * 0.866025403784439};
}

bool is_valid(Coordinates c, int size) {
    return ((c.x + c.y) % 2 == 0) && 
        (abs(c.y) + abs(c.x) <= 2 * (size - 1)) &&
        (abs(c.x) <= 2 * (size - 1)) &&
        (abs(c.y) <= (size - 1));
}
std::vector<Coordinates> const possible_connections = {{0,0},{1,1},{2,0},{1,-1},{-1,-1},{-2,0},{-1,1}};

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc,args);
    cp[1].check_present("Must specify world name").check_file_exist(".world");
    int64_t p_seed = cp["-seed"].default_value(-1).check_range(-1,65535);
    int64_t p_size = cp["-size"].default_value(11).check_range(5,19);
    int64_t p_occlusions = cp["-occlusions"].default_value(80).check_range(1,200);
    string world_name(cp[1]);
    set_seed(p_seed);
    int8_t size = p_size;
    int occlusions = p_occlusions;
    unsigned int prey, predator, goal;
    World world (world_name);
    Coordinates start_coordinates{(int8_t)-size,0}, goal_coordinates{0,0}, predator_coordinates{size,0};
    for ( int8_t y = -10 ; y <= 10; y++ ){
        for ( int8_t x = -20 ; x <= 40; x++ ){
            Coordinates coordinates{x,y};
            if (is_valid(coordinates,size)) {
                Cell c(Circle, coordinates, get_location(coordinates), 0, false);
                world.add(c);
            }
        }
    }
    //set occlusions
    for(int i=0;i<occlusions;){
        unsigned int r = rand() % world.size();
        if (!world[r].occluded && 
            !(world[r].coordinates == start_coordinates) &&
            !(world[r].coordinates == goal_coordinates) &&
            !(world[r].coordinates == predator_coordinates)){
            world.set_occlusion(r,true);
            i++;
        }
    }
    cout << world.size() << endl;
    world.save();
    cout << "done" << endl;
}