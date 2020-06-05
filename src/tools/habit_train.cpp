#include <cell_world.h>
#include <iostream>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/habit_training_prey.h"

using namespace cell_world;
using namespace std;

void run_training(uint32_t thread);
double show_progress(uint32_t episodes, uint32_t &episode);


struct Thread_data{
    Thread_data(Cell_group c, Paths &p, uint32_t es,uint32_t s,uint32_t &e,uint32_t &su, Graph &g):
            cells(c), paths(p), episodes(es), steps(s), episode(e), success(su), graph(g){};
    Cell_group cells;
    Paths &paths;
    uint32_t episodes;
    uint32_t steps;
    uint32_t &episode;
    uint32_t &success;
    Graph &graph;
    vector<Habit> habits;
    bool debug = false;
};

mutex mtx;
vector<Thread_data> threads_data;
Reward_config rc {100,-100,-100, .99,-1};

int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    cp[1].check_present().check_file_exist(".world");
    int64_t p_seed = cp["-seed"].int_value(-1);
    uint32_t steps = cp["-steps"].int_value(100);
    uint32_t episodes = cp["-episodes"].int_value(1);
    int64_t threads = cp["-debug"].present()?1:cp["-threads"].int_value(1);
    set_seed(p_seed);
    string world_name (cp[1]);
    World world(world_name);
    world.load();
    Paths paths = world.create_paths(world_name, cell_world::Paths::Path_type::shortest);
    create_folder(world_name);
    Cell_group world_cells = world.create_cell_group();
    Graph world_graph = world.connection_pattern.get_graph(world_cells);
    Cell_group gates_cells = world.create_cell_group( world_name + "_gates" );
    Graph gates_graph(gates_cells);
    Graph gate_connections(world_cells);
    vector<Habit> world_habits = Habit::get_habits(world_graph, gates_graph, world_name);
    uint32_t episode = 0;
    uint32_t success = 0;
    if (threads>world_habits.size()) threads = world_habits.size();
    for (uint32_t i = 0; i < threads; i++)
        threads_data.emplace_back(world_cells, paths, episodes, steps, episode, success, world_graph);
    for (uint32_t i = 0; i < world_habits.size(); i++) {
        threads_data[i % threads].habits.push_back(world_habits[i]);
    }
    if (cp["-debug"].present()) {
        threads_data[0].debug = true;
        run_training(0);
    } else {
        Stop_watch iteration_watch;
        if (threads > 1) {
            vector<thread> t;
            for (uint32_t i = 0; i < threads; i++)
                t.emplace_back(run_training, i);
            show_progress(episodes, episode);
            for (auto &th: t)th.join();
        } else {
            run_training(0);
        }
        double elapsed = iteration_watch.elapsed();
        cout << "Success rate: " << (double) success / (double) episodes * 100.0 << " execution time: " << Stop_watch::to_string(elapsed) << "(" <<(double)episode/elapsed << " episodes per sec.)" << endl;
    }
    for(auto &thread_data:threads_data) for(auto &habit:thread_data.habits) habit.save(world_name);
}

void run_training(uint32_t thread){
    auto &data = threads_data[thread];
    Model m(data.cells);
    Predator predator(data.graph, m.visibility, data.paths, rc);
    Habit_training_prey ht(data.habits, rc,.9);
    m.add_agent(ht);
    m.add_agent(predator);
    for (;;) {
        mtx.lock();
        if (data.episode < data.episodes) {
            data.episode++;
            mtx.unlock();
        } else {
            mtx.unlock();
            break;
        }
        if (data.debug){
            Simulation s(m,{1024,768},data.steps);
            s.run();
        } else {
            m.start_episode();
            for (uint32_t i = 0; i < data.steps && m.update(); i++);
            m.end_episode();
        }
    }
    mtx.lock();
    data.success += ht.successes;
    mtx.unlock();
}

double show_progress(uint32_t episodes, uint32_t &episode){
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
            cout << "\r" << bar << (progress<10?" ":"") << progress << "% (" << episode << "/" << episodes << ") - " << (int)elapsed << "s (" <<(double)episode/elapsed << " episodes per sec.)    " << flush;
        }
        if (episode >= episodes) break;
    }
    double elapsed = iteration_watch.elapsed();
    cout << "\r|===================================================| 100% (" << episodes << "/" << episodes << ")- " << (int)elapsed << "s (" <<(double)episode/elapsed << " episodes per sec.)" << endl;
    return iteration_watch.elapsed();
}