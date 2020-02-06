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
    if (argc == 1) {
        print_hexaworld_help();
        exit(0);
    }
    {
        struct stat buffer;
        string filename (args[1]);
        if (stat ((filename + ".world").c_str(), &buffer)) {
            cout << "'" << args[1] << "': No such file or directory" << endl;
            exit(0);
        }
    }
    string world_name (args[1]);
    int64_t p_seed = get_parameter_int("-seed", -1, argc, args);
    set_seed(p_seed);
    bool exploration = get_parameter("-exploration", "false", argc, args)=="true";
    bool show = get_parameter("-show", "false", argc, args)=="true";
    uint16_t steps = get_parameter_int("-steps", 1000, argc, args);
    uint32_t episodes = get_parameter_int("-episodes", 1, argc, args);
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    string input_file  = get_parameter("-in", "partial_vision.", argc, args);
    string output_file  = get_parameter("-out", "partial_vision.", argc, args);
    World world(world_name);
    world.load();
    Cell_group cg = world.create_cell_group();
    Chance pp(PREDATOR_PROBABILITIES);
    Predator predator(world,pp);
    Test_prey tp(cg);
    vector<Agent*> va;
    va.push_back((Agent*)&predator);
    va.push_back((Agent*)&tp);
    Simulation c(world, va, {width, height}, steps, episodes);
    c.run();
    cout << endl;
}