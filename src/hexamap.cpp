#include <iostream>
#include <fstream>
#include <ge211.h>
#include <cellworld.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace ge211;
using namespace std;
using namespace cellworld;

int const arena_side = 11;

Basic_position<double> get_location(Coordinates c) {
    return {(double)c.x *  .5 ,  (double)c.y * 0.866025403784439};
}

double distance(Coordinates const p0, Coordinates const p1) {
    ge211::Basic_position<double> c1 = get_location(p1);
    ge211::Basic_position<double> c0 = get_location(p0);
    return sqrt(pow(c1.y-c0.y,2) + pow(c1.x-c0.x,2));
}

double distance(Coordinates const p0, Coordinates const p1, Coordinates const p2) {
    ge211::Basic_position<double> c1 = get_location(p1);
    ge211::Basic_position<double> c2 = get_location(p2);
    ge211::Basic_position<double> c0 = get_location(p0);
    return abs((c2.y-c1.y) * c0.x-(c2.x-c1.x) *c0.y+c2.x*c1.y-c2.y * c1.x) / sqrt(pow(c2.y-c1.y,2)+pow(c2.x-c1.x,2));
}

bool is_valid(Coordinates c) {
    return ((c.x + c.y) % 2 == 0) && 
        (abs(c.y) + abs(c.x) <= 2 * (arena_side - 1)) && 
        (abs(c.x) <= 2 * (arena_side - 1)) && 
        (abs(c.y) <= (arena_side - 1));
}
std::vector<Coordinates> const posible_connections = {{0,0},{1,1},{2,0},{1,-1},{-1,-1},{-2,0},{-1,1}};

Coordinates get_cell_coordinates(ge211::Basic_position<int> map_indexes){
    return { (int8_t) (map_indexes.x - 2 * (arena_side - 1)), (int8_t) (map_indexes.y - (arena_side - 1))};
}
int get_parameter(char *arg)
{
    std::stringstream str(arg);
    int x;  
    str >> x;
    return x;
}

int main (int argc, char *args[]) {
    World world;
    string filepath = "output.dat";
    int occlussions=80;
    Coordinates startc{-20,0}, goalc{20,0}, predatorc{0,0};
    if (argc>1){
        srand(get_parameter(args[1]));
    } else {
        struct timeval tp;
        gettimeofday(&tp, NULL); 
        srand (tp.tv_usec);
    }        
    if (argc>2) filepath = args[3];
    if (argc>3) occlussions = get_parameter(args[3]);
    if (argc>4) startc.x = get_parameter(args[4]);
    if (argc>5) startc.y = get_parameter(args[5]);
    if (argc>6) goalc.x = get_parameter(args[6]);
    if (argc>7) goalc.y = get_parameter(args[7]);
    if (argc>8) predatorc.x = get_parameter(args[8]);
    if (argc>9) predatorc.y = get_parameter(args[9]);

    unsigned int prey, predator, goal;
    Coordinates start_coordinates{0,0}, goal_coordinates{0,0}, predator_coordinates{0,0};
    for ( int8_t y = -10 ; y <= 10; y++ ){
        for ( int8_t x = -20 ; x <= 40; x++ ){
            Coordinates coordinates{x,y};
            if (is_valid(coordinates))
                world.add({(uint32_t)world.size(), coordinates, get_location(coordinates), {}, 0, false });
        }
    }
    //set occlussions
    for(int i=0;i<occlussions;){
        unsigned int r = rand() % world.size();
        if (!world[r].occluded && 
            !(world[r].coordinates==startc) && 
            !(world[r].coordinates==goalc) && 
            !(world[r].coordinates==predatorc)){
            world[r].occluded=true;
            i++;
        }
    }
    world.connect(posible_connections);
    world.save(filepath);
}