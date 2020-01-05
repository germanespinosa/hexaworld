#include <cellworld.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "predator.h"
#include "blind.h"
#include "partial_vision.h"
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
        string filename (args[1]);
        if (stat ((filename + ".map").c_str(), &buffer)) {
            cout << "'" << args[1] << "': No such file or directory" << endl;
            exit(0);
        }
    }
    string world_name (args[1]);
    int64_t p_seed = get_parameter_int("-seed", -1, argc, args);
    set_seed(p_seed);
    bool exploration = get_parameter("-exploration", "false", argc, args)=="true";
    bool show = get_parameter("-show", "false", argc, args)=="true";
    uint16_t steps = get_parameter_int("-steps", 100, argc, args);
    uint32_t episodes = get_parameter_int("-episodes", 1, argc, args);
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    string input_file  = get_parameter("-in", "partial_vision.", argc, args);
    string output_file  = get_parameter("-out", "partial_vision.", argc, args);
    World world(world_name);
    world.load();
    Connections wc;
    world.get_connections(wc , ADJACENT_CELLS);
    cout << "convergence : " << wc.process_eigen_centrality(1000, .000001) << endl;
    double max = 0;
    double sum = 0;
    for (uint32_t i = 0 ; i < world.size(); i++) {
        if (max < wc[i].eigen_centrality)max = wc[i].eigen_centrality;
        sum += wc[i].eigen_centrality;
    }
    cout << "max eigencentrality: "<< max <<endl;
    cout << "sum eigencentrality: "<< sum <<endl;
    for (uint32_t i = 0 ; i < world.size(); i++)
        world.set_value(i,wc[i].eigen_centrality/max);
    Visibility vi(world);
    Predator predator(world, vi);
    Prey_config config;
    config.start = world[{-20,0}].id;
    config.goal = world[{20,0}].id;
    config.success_reward = 1000;
    config.failure_reward = 0;
    config.discount = 1;
    config.step_cost = -1;
    config.color = Green;
    cout << "Exploration mode: ";
    if (exploration) {
        cout << "ON" << endl;
        config.action_probabilities = {50, 75, 80, 85, 90, 95, 100};
    }
    else {
        cout << "OFF" << endl;
        config.action_probabilities = {80, 95, 96, 97, 98, 99, 100};
    }
    Partial_vision prey(world, vi, config);
    vector<Agent*> va;
    va.push_back((Agent*)&predator);
    va.push_back((Agent*)&prey);
    if ( show ) {
        Simulation c(world, va, {width, height}, steps, episodes);
        c.run();
    }else{
        Model m(world,va);
        int perc = 0;
        for (uint32_t episode = 1 ; episode <= episodes; episode++ ) {
            if (episode % (episodes/100) == 0)  cout << "\rProgress: " << ++perc << "% episode: " << episode << flush;
            m.start_episode();
            for (uint32_t i = 0; i < steps &&  m.update(); i++);
            m.end_episode();
        }
    }
    cout << endl;
//    world.save("heatmap.dat");
}