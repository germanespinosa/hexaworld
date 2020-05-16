#include <iostream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/habit_training_prey.h"
#include <time.h>   	// for clock_t, clock(), CLOCKS_PER_SEC

using namespace ge211;
using namespace std;
using namespace cell_world;

int main (int argc, char *args[]) {
    clock_t begin = clock();
    Cmd_parameters cp(argc,args);
    string world_name("square_world");
    create_folder(world_name);
    World world(world_name);
    world.load();
    Cell_group cg = world.create_cell_group();
    Graph graph = world.create_graph();
    Graph vi = Visibility::create_graph(cg);
    Paths paths = world.create_paths(world_name, cell_world::Paths::Path_type::shortest);
    Predator predator(graph,vi, paths);
    Map map(graph.nodes);
    Option option{graph,map[{0,7}],};
    Habit habit(option);
    Reward_config rc {100,-100,-100, .99,-1};
    vector<Habit> habits;
    habits.push_back(habit);
    habits[0].load(world_name);
    Habit_training_prey ht(habits, rc, .9);
    if (cp["-fpreds"].present()) {
        cout << "Fixed predator location - " << flush;
        predator.set_fixed_start(map[{0,0}]);
    }
    if (cp["-seedvalues"].present()) cout << "Seed values - " << flush;
    Model m(cg);
    m.add_agent(predator);
    m.add_agent(ht);
    m.iterations = cp["-s"].int_value(60);
    Simulation simulation (m, {1000,1000},cp["-e"].int_value(100000));
    if (cp["-silent"].present())
        simulation.run_silent(true);
    else
        simulation.run();
    habits[0].save(world_name);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << " Success: " << round((double)ht.successes/(double)ht.episodes * 100,2)
    << "% - Total runtime: " << round(time_spent,2) << "s ("
    << round((double)cp["-e"].int_value(100000)/ time_spent,2) << " episodes/s)" << endl;
}
