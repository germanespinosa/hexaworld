#include <cell_world.h>
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
int64_t convergence_limit = 0;
vector<Habit> pending_habits;
Chance exploration(vector<uint32_t>{95,1,1,1,1,1});
Chance sm(vector<uint32_t>{95,1,1,1,1,1});
Reward_config rc {100,-100,0, .99,-1};
uint32_t episode = 0;
World *P_world = nullptr;
Cell_group *P_cg= nullptr;
Action_set *P_aas = nullptr;
uint32_t success=0;

void run_train(uint32_t thread){
    Predator predator(*P_world);
    vector<Agent*> va;
    Habit_training ht(pending_habits, rc,*P_aas, exploration, thread, threads);
    va.push_back(&predator);
    va.push_back(&ht);
    Model m(*P_cg,va);
    for (;;) {
        mtx.lock();
        if (episode < episodes) {
            episode++;
            mtx.unlock();
        } else {
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
    Cmd_parameters cp(argc,args);
    if (!cp[1].present()) {
        print_hexaworld_help();
        exit(0);
    }
    {
        struct stat buffer{};
        string filename = cp[1].value();
        if (stat ((filename + ".world").c_str(), &buffer)) {
            cout << "'" << args[1] << "': No such file or directory" << endl;
            exit(0);
        }
    }
    string world_name (args[1]);
    convergence_limit = cp["-convergence"].int_value(1000);
    int64_t p_seed = cp["-seed"].int_value(-1);
    set_seed(p_seed);
    bool show = cp["-show"].present();
    steps = cp["-steps"].int_value(100);
    episodes = cp["-episodes"].int_value(1);
    threads = cp["-threads"].int_value(1);
    World world(world_name);
    world.load();
    create_folder(world_name);
    Cell_group cg = world.create_cell_group();
    Visibility vi(cg);
    Connections cnn(cg,world.connection_pattern);
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
    Action_set aas(cg,world.connection_pattern,sm);
    uint32_t ready = 0;
    for (auto &habit:habits){
        bool converged = true;
        habit.load(world_name, aas.size());
        for (uint32_t i=0;i<habit.cells.size();i++){
            if (habit.values[i].get_visits()<convergence_limit){
                //cout<< "pending " << habit.sub_world_id << " cell "<< i << " " << !habit.cells[i].coordinates << " with " << habit.values[i].get_visits() << " visits" << endl;
                converged = false;
                break;
            }
        }
        habit.convergence = converged;
        if(converged) {
            ready++;
        } else {
            pending_habits.push_back(habit);
        }
    }
    cout << "Converged habits: " << ready << " of " << habits.size() << endl;
    if (ready == habits.size()) {
        cout << "no pending work." << endl;
        return 0;
    }
    for (auto &habit:pending_habits){
        habit.load(world_name, aas.size());
    }
    if (pending_habits.size()<threads)threads = pending_habits.size();

    P_world = &world;
    P_aas = &aas;
    P_cg = &cg;
    vector<thread> t;
    for (uint32_t i =0; i<threads; i++)
        t.emplace_back(run_train,i);
    string bar ("|---------------------------------------------------|  ");
    int32_t perc = episodes / 100;
    int32_t progress = -1;
    Stop_watch iteration_watch;
    for (;;) {
        int32_t p = perc > 0 ? episode/perc : 100;
        if (p>progress) {
            progress = p;
            bar[progress/2+1]='=';
            bar[progress/2+2]='>';
            double elapsed = iteration_watch.elapsed();
            cout << "\r" << bar << (progress<10?" ":"") << progress << "% (" << episode << "/" << episodes << ") - " << (int)elapsed << "s (" <<(double)episode/elapsed << " episodes per sec.)" << flush;
        }
        if (episode >= episodes) break;
    }
    for (uint32_t i =0; i<threads; i++)
        t[i].join();
    double elapsed = iteration_watch.elapsed();
    cout << "\r|===================================================| 100% (" << episodes << "/" << episodes << ")- " << (int)elapsed << "s (" <<(double)episode/elapsed << " episodes per sec.)" << endl;
    for (auto &habit:pending_habits){
        habit.save(world_name);
    }
    cout << "\rSuccess rate: " << (double) success / (double) episodes * 100.0 << "%" << endl << "Total execution time: " << Stop_watch::to_string(iteration_watch.elapsed()) << endl;
}