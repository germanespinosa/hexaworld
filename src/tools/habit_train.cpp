#include <cellworld.h>
#include <iostream>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "utils.h"
#include "predator.h"
#include "hexaworld.h"
#include <sys/stat.h>
#include "habit_training.h"

using namespace cell_world;
using namespace std;

mutex mtx;

uint32_t steps = 0;
uint32_t episodes = 0;
uint32_t threads = 0;
vector<Habit> habits;
Probabilities exploration(vector<uint32_t>{95,1,1,1,1,1});
Probabilities sm(vector<uint32_t>{95,1,1,1,1,1});
Reward_config rc {100,-100,0, .99,-1};
uint32_t episode = 0;
World *P_world = NULL;
Action_set *P_aas = NULL;
uint32_t success=0;

void run_train(uint32_t thread){
    Probabilities pp(PREDATOR_PROBABILITIES);
    Predator predator(*P_world,pp);
    vector<Agent*> va;
    Habit_training ht(habits, rc,*P_aas, exploration, thread, threads);
    va.push_back(&predator);
    va.push_back(&ht);
    Model m(*P_world,va);
    for (;;) {
        mtx.lock(); if (episode < episodes) {
            episode++;
            mtx.unlock();
        }
        else {
            mtx.unlock();
            break;
        }
        m.start_episode();
        for (uint32_t i = 0; i < steps &&  m.update(); i++);
        m.end_episode();
    }
    mtx.lock();
    success += ht.success;
    mtx.unlock();
}

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
    bool show = get_parameter("-show", "false", argc, args)=="true";
    steps = get_parameter_int("-steps", 100, argc, args);
    episodes = get_parameter_int("-episodes", 1, argc, args);
    threads = get_parameter_int("-threads", 1, argc, args);
    World world(world_name);
    world.load();
    create_folder(world_name);
    Visibility vi;
    Cell_group cg = world.create_cell_group();
    vi.reset(cg);
    Connections cnn(cg,ADJACENT_CELLS);
    Cell_group gates = world.create_cell_group( world_name + "_gates" );
    Sub_worlds sws(cg, gates, cnn);
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
    Action_set aas(cg,ADJACENT_CELLS,sm);
    for (auto &habit:habits){
        habit.load(world_name, aas.size());
    }
    P_world = &world;
    P_aas = &aas;
    vector<thread> t;
    for (uint32_t i =0; i<threads; i++)
        t.push_back(thread(run_train,i));
    int32_t perc = episodes / 100;
    int32_t progress = -1;
    while ( episode < episodes) {

        int32_t p= perc>0?episode/perc:100;
        if (p>progress) {
            progress = p;
            cout << "\rProgress: " << progress << "% episode: " << episode << flush;
        }
    }
    for (uint32_t i =0; i<threads; i++)
        t[i].join();
    cout << "\rProgress: 100% episode: " << episode << endl << "Saving habits" << endl;
    for (uint32_t i=0;i<habits.size();i++){
        habits[i].save(world_name);
    }
    cout << "\rSuccess rate: " << (double) success / (double) episodes * 100.0 << "%" << endl;
}