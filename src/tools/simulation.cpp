#include <cell_world.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "predator.h"
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
    {
        struct stat buffer;
        string filename (args[1]);
        if (stat ((filename + ".world").c_str(), &buffer)) {
            cout << "'" << cp[1].value() << "': No such file or directory" << endl;
            exit(0);
        }
    }
    string world_name (args[1]);
    int64_t p_seed = cp["-seed"].int_value(-1);
    set_seed(p_seed);
    bool exploration = cp["-exploration"].present();
    bool show = cp["-show"].present();
    uint16_t steps = cp["-steps"].int_value(1000);
    uint32_t episodes = cp["-episodes"].int_value(1);
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    string input_file  = cp["-in"].value("partial_vision");
    string output_file  = cp["-out"].value("partial_vision.");
    World world(world_name);
    world.load();
    Cell_group cg = world.create_cell_group();
    Predator predator(world);
    Test_prey tp(world);
    vector<Agent*> va;
    va.push_back((Agent*)&predator);
    va.push_back((Agent*)&tp);
    Simulation c(cg, va, {width, height}, steps, episodes);
    c.run();
    cout << endl;
}