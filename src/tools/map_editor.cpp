#include <cellworld.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "hexaworld.h"
#include <sys/stat.h>

using namespace cellworld;
using namespace std;

int main(int argc, char *args[]){
    if (argc == 1) {
        print_hexaworld_help();
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
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    string input_file  = get_parameter("-in", "partial_vision.", argc, args);
    string output_file  = get_parameter("-out", "partial_vision.", argc, args);
    World world;
    world.load(filename);
    World_connections wc(world, ADJACENT_CELLS);
    cout << "convergence : " << wc.process_eigen_centrality(1000, .000001) << endl;
    double max = 0;
    double sum = 0;
    for (uint32_t i = 0 ; i < world.size(); i++) {
        if (max < wc.eigen_centrality[i])max = wc.eigen_centrality[i];
        sum += wc.eigen_centrality[i];
    }
    for (uint32_t i = 0 ; i < world.size(); i++)
        world.set_value(i,wc.eigen_centrality[i]/max);
    Visibility vi(world);
    Map_editor c(world, {width, height});
    c.run();

}