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
    Sub_worlds sws(cg, gates, cnn);
    vector<Habit> habits;
    for ( uint32_t i = 0; i < sws.size(); i++){
        auto sw = sws[i];
        for ( uint32_t j = 0; j < sw.gate_cells.size(); j++){
            Habit habit;
            habit.convergence = false;
            habit.sub_world_id = i;
            habit.destination = sw.gate_cells[j];
            habit.cells = sw.cells - habit.destination;
            habit.gate_cells = sw.gate_cells;
            habits.push_back(habit);
        }
    }
    Probabilities p(vector<uint32_t>{100,50,25,10,10,10});
    Action_set aas(cg,ADJACENT_CELLS,p);
    for (auto &habit:habits){
        habit.load(world_name, aas.size());
    }
    Predator predator(cg,vi);
    vector<Agent*> va;
    Reward_config rc {100,-100,.99,1};
    Habit_training ht(habits,rc,aas, p);
    va.push_back(&predator);
    va.push_back(&ht);
    Model m(world,va);
    int perc = 0;
    for (uint32_t episode = 1 ; episode <= episodes; episode++ ) {
        if ((episodes/100)>0 && episode % (episodes/100) == 0)  cout << "\rProgress: " << ++perc << "% episode: " << episode << flush;
        m.start_episode();
        for (uint32_t i = 0; i < steps &&  m.update(); i++);
        m.end_episode();
    }
    cout << endl << "saving habits"<< endl;
    for (uint32_t i=0;i<habits.size();i++){
        habits[i].save(world_name);
    }
}