#include <iostream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"
#include "hexaworld.h"
#include "predator.h"
#include "habit_training.h"
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
    Visibility vi;
    Cell_group cg = world.create_cell_group();
    vi.reset(cg);
    Map map(cg);
    Chance pp(vector<uint32_t>{80,10,5,5});
    Predator predator(world);
    vector<Agent*> va;
    Habit habit;
    habit.destination = map[{7,0}];
    habit.cells = cg;
    habit.gate_cells += habit.destination;
    Chance sm(vector<uint32_t>{80,10,5,5});
    Chance exploration(vector<uint32_t>{80,10,5,5});
    Action_set aas(cg,world.connection_pattern,sm);
    Reward_config rc {100,-100,-100, .99,-1};
    vector<Habit> habits;
    habits.push_back(habit);
    habits[0].load(world_name,world.connection_pattern.size());
    Habit_training ht(habits, rc,aas, exploration, 0, 1);
    if (cp["-fpreys"].present()) {
        cout << "Fixed prey location - " << flush;
        ht.set_fixed_start(map[{-7,0}]);
    }
    if (cp["-fpreds"].present()) {
        cout << "Fixed predator location - " << flush;
        predator.set_fixed_start(map[{0,0}]);
    }
    predator.set_view_range(5);
    if (cp["-seedvalues"].present()) cout << "Seed values - " << flush;

    va.push_back(&predator);
    va.push_back(&ht);
    Simulation simulation (cg, va, {1000,1000},cp["-s"].int_value(60),cp["-e"].int_value(100000));
    cout << "here " << endl;
    if (cp["-silent"].present())
        simulation.run_silent(true);
    else
        simulation.run();
    cout << "here2 " << endl;

    if (cp["-seedvalues"].present()) habits[0].clear_stats();

    habits[0].save(world_name);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << " Success: " << round((double)ht.success/(double)ht.episodes * 100,2)
    << "% - Total runtime: " << round(time_spent,2) << "s ("
    << round((double)cp["-e"].int_value(100000)/ time_spent,2) << " episodes/s)" << endl;
}
