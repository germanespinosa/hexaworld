#include <iostream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"
#include "hexaworld.h"

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
    if (!cp[1].present()) {
        print_hexamap_help();
        exit(0);
    }
    string world_name(cp[1].value());
    int64_t p_seed = cp["-seed"].int_value(-1);
    if (p_seed < -1 || p_seed > 65535){
        cerr << "Incorrect value for parameter seed (\"" << p_seed << "\")" << endl;
        print_hexamap_help();
        exit(0);
    }
    set_seed(p_seed);
    int64_t p_size = cp["-size"].int_value(11);
    if (p_size < 5 || p_size > 19 || p_size % 2 == 0) {
        cerr << "Incorrect value for parameter size (\"" << p_size << "\")" << endl;
        print_hexamap_help();
        exit(0);
    }
    int8_t size = p_size;
    int64_t p_occlusions = cp["-occlusions"].int_value(80);
    if (p_occlusions < 1 || p_occlusions > 200) {
        cerr << "Incorrect value for parameter occlusions (\"" << p_occlusions << "\")" << endl;
        print_hexamap_help();
        exit(0);
    }
    int occlusions = p_occlusions;

    unsigned int prey, predator, goal;
    World world (world_name);

    Coordinates start_coordinates{(int8_t)-size,0}, goal_coordinates{0,0}, predator_coordinates{size,0};
    for ( int8_t y = -10 ; y <= 10; y++ ){
        for ( int8_t x = -20 ; x <= 40; x++ ){
            Coordinates coordinates{x,y};
            if (is_valid(coordinates,size))
                world.add({(uint32_t)world.size(), coordinates, get_location(coordinates), 0, false });
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
    Connections wc( world.create_cell_group(), ADJACENT_CELLS);
    wc.save(world_name);
    cout << "done" << endl;
}