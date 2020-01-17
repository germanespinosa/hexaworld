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
        if (stat (( world_name + ".world").c_str(), &buffer)) {
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
    Cell_group cg = world.create_cell_group();
    Sub_worlds sw(cg);
    Connections cn(cg,ADJACENT_CELLS);
    Cell_group selected = world.create_cell_group(world_name);
    Map_editor c(world, {width, height},ADJACENT_CELLS);
    //sw.reset(cg, selected, cn);
    //sw.reset(cg, selected, cn);
    c.run();
}