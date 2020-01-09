#include <cellworld.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "hexaworld.h"
#include <sys/stat.h>

using namespace cell_world;
using namespace std;

int main(int argc, char *args[]){
    if (argc == 1) {
        print_hexaworld_help();
        exit(0);
    }
    string world_name (args[1]);
    {
        struct stat buffer;
        if (stat (( world_name + ".map").c_str(), &buffer)) {
            cout << "'" << args[1] << "': No such file or directory" << endl;
            exit(0);
        }
    }
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    string input_file  = get_parameter("-in", "partial_vision.", argc, args);
    string output_file  = get_parameter("-out", "partial_vision.", argc, args);
    World world(world_name);
    world.load();
    Sub_worlds sw;
    Cell_group bridges = world.create_cell_group (world_name + ".sel");
    Connections cn;
    world.create_cell_group().get_connections(cn,CONTACT_CELLS);
    sw.reset(world, bridges, cn);
    sw.reset_connections();
}