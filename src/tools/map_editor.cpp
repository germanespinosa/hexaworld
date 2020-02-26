#include <cell_world.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "hexaworld.h"
#include <sys/stat.h>

using namespace cell_world;
using namespace std;

int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    if (!cp[1].present()) {
        print_hexaworld_help();
        exit(0);
    }
    string world_name (args[1]);
    {
        struct stat buffer;
        if (stat (( world_name + ".world").c_str(), &buffer)) {
            cout << "'" << cp[1].value() << "': No such file or directory" << endl;
            exit(0);
        }
    }
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    World world(world_name);
    world.load();
    Cell_group cg = world.create_cell_group();
    Cell_group selected = world.create_cell_group(world_name);
    Map_editor c(world, {width, height});
    c.run();
}