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

    /*for (int16_t y = -7; y <= 7; y++) {
        for (int16_t x = -7; x <= 7; x++) {
            Coordinates coordinates{(int8_t )x,(int8_t ) y};
            Location location{(double)x,(double) y};
            world.add({(uint32_t) world.size(), coordinates, location, 0, false});
        }
    }
    world.add({(uint32_t) world.size(), {8,0} , {8.0,0.0}, 0, false});
     */
    cout<< "here" << endl;
    world.connection_pattern = Connection_pattern ({{-1,0},{1,0},{0,-1},{0,1}});
    Visibility vi;
    Cell_group cg = world.create_cell_group();
    vi.reset(cg);
    Map map(cg);
    cout<< "here1" << endl;

    Chance pp(vector<uint32_t>{80,10,5,5});
    Predator predator(world,pp);
    cout<< "here1" << endl;
    vector<Agent*> va;
    cout<< "here2" << endl;
    Habit habit;
    cout<< "here2" << endl;
    habit.destination = map[{7,0}];
    cout<< "here2" << endl;
    habit.cells = cg;
    habit.gate_cells += habit.destination;
    cout<< "here1" << endl;
    Chance sm(vector<uint32_t>{80,10,5,5});
    cout<< "here1" << endl;
    Chance exploration(vector<uint32_t>{80,10,5,5});
    cout<< "here1" << endl;
    Action_set aas(cg,world.connection_pattern,sm);
    cout<< "here1" << endl;
    Reward_config rc {100,-100,-100, .99,-1};
    cout<< "here1" << endl;
    vector<Habit> habits;
    habits.push_back(habit);
    cout<< "here1" << endl;
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

    if (find_parameter("-seedvalues", argc,args)) cout << "Seed values - " << flush;

    va.push_back(&predator);
    va.push_back(&ht);
    Simulation simulation (world, va, {1000,1000},cp["-s"].int_value(60),cp["-e"].int_value(100000));
    if (find_parameter("-silent", argc,args))
        simulation.run_silent(true);
    else
        simulation.run();
    if (find_parameter("-seedvalues", argc,args)) habits[0].clear_stats();

    habits[0].save(world_name);
    world.save();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << " Success: " << round((double)ht.success/(double)ht.episodes * 100,2)
    << "% - Total runtime: " << round(time_spent,2) << "s ("
    << round((double)cp["-e"].int_value(100000)/ time_spent,2) << " episodes/s)" << endl;
}
