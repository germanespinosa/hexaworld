#include <iostream>
#include <cell_world.h>
#include "utils.h"

using namespace std;
using namespace cell_world;

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc, args);
    string world_name(cp[1].value());
    World world(world_name);
    world.load();
    Graph g = world.create_graph();
    Cell_group spawning_locations;
    while (spawning_locations.size()<cp[2].int_value(5)){
        spawning_locations.add(g.nodes.random_cell());
    }
    cout << '"';
    for (uint32_t i = 0;i<spawning_locations.size();i++){
        if (i) cout << " ";
        cout<< "'(" << (int) spawning_locations[i].coordinates.x+7 <<";" << (int) (-spawning_locations[i].coordinates.y)+7 << ")'";
    }
    cout << '"';
}
