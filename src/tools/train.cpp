#include <cellworld.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "predator.h"
#include "hexaworld.h"
#include <sys/stat.h>
#include "habit_training.h"

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
    uint16_t steps = get_parameter_int("-steps", 100, argc, args);
    uint32_t episodes = get_parameter_int("-episodes", 1, argc, args);
    int width = get_parameter_int("-width", 1024, argc, args);
    int height = get_parameter_int("-height", 768, argc, args);
    string input_file  = get_parameter("-in", "partial_vision.", argc, args);
    string output_file  = get_parameter("-out", "partial_vision.", argc, args);
    World world(world_name);
    world.load();
    Visibility vi(world);
    Cell_group cg = world.create_cell_group();
    Connections cnn(cg,ADJACENT_CELLS);
    Cell_group gates = world.create_cell_group( world_name + "_gates" );
    cout << "gates: " << gates.size() << endl;
    Sub_worlds sw(cg, gates, cnn);
    vector<Habit> habits;
    for (auto g : sw.gates)
    {
        for (auto gc:g.gate_connections)
        {
            uint32_t index;
            for( index = 0;
                index < habits.size() &&
                !( habits[index].source.id == g.cell.id && habits[index].destination.id == gc.destination.id );
                index++);
            if (index == habits.size()){
                habits.emplace_back(g.cell,gc.destination);
            }
            auto habit = habits[index];
            habit.connections.cells += gc.sub_world.cells;
            habit.gate_cells += gc.sub_world.gate_cells;
        }
    }
    for (uint32_t i=0;i<habits.size();i++){
        habits[i].connections.reset(ADJACENT_CELLS);
        habits[i].load(world_name);
    }
    Predator predator(cg,vi);
    vector<Agent*> va;
    Probabilities p(vector<uint32_t>{100,30,20,10,10,10});
    Reward_config rc {100,-100,.99,1};
    Habit_training ht(habits,rc,p);
    va.push_back(&predator);
    //va.push_back(&ht);
    Model m(world,va);
    int perc = 0;
    for (uint32_t episode = 1 ; episode <= episodes; episode++ ) {
        if ((episodes/100)>0 && episode % (episodes/100) == 0)  cout << "\rProgress: " << ++perc << "% episode: " << episode << flush;
        m.start_episode();
        for (uint32_t i = 0; i < steps &&  m.update(); i++);
        m.end_episode();
    }
    cout << endl;
    for (uint32_t i=0;i<habits.size();i++){
        habits[i].save(world_name);
    }
}