#include <cellworld.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "predator.h"
#include "prey.h"

#include <sys/stat.h>

using namespace cellworld;
using namespace std;

int main(int argc, char *args[]){
    if (argc == 1) {
        print_help();
        exit(0);
    }
    {
        struct stat buffer;
        if (stat (args[1], &buffer)) {
            cout << "'" << args[1] << "': No such file or directory" << endl;
            exit(0);
        }
    }
    string filename = args[1];
    bool show = get_parameter("-show", "true", argc, args)=="true";
    uint16_t steps = get_parameter_int("-episodes", 100, argc, args);
    uint32_t episodes = get_parameter_int("-episodes", 1, argc, args);
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    for (uint32_t episode = 0 ; episode < episodes; episode++ ) {
        World world;
        world.load(filename);
        Visibility vi(world);
        Predator predator(world, vi, 1);
        Prey prey(world, vi, 0);
        vector<Agent*> va;
        va.push_back(&predator);
        va.push_back(&prey);
        if (show) {
            Controller c(world, va, {width, height}, steps);
            c.run();
        } else {
            Model m(world,va);
            for (uint32_t i = 0; i < steps ; i++){
                m.update();
            }
        }
        world.save("heatmap.dat");
    }
}