#include <iostream>
#include <cell_world.h>
#include "utils.h"

using namespace std;
using namespace cell_world;

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc,args);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    Paths euclidean = world.create_paths(Paths::Path_type::euclidean);
    Paths shortest = world.create_paths(Paths::Path_type::shortest);
    euclidean.save();
    shortest.save();
}