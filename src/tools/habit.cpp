#include <cell_world.h>
#include <iostream>
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/planners/habit_planner.h"
//11 60 90
using namespace cell_world;
using namespace std;

int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    cp[1].check_present().check_file_exist(".world");
    int64_t p_seed = cp["-seed"].default_value(-1).check_range(-1,65535).int_value();
    uint16_t steps = cp["-steps"].int_value(80);
    uint32_t episodes = cp["-episodes"].int_value(1);
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    set_seed(p_seed);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    for (uint32_t i=0;i<world.size();i++)world[i].value = 0;
    world.save();
    auto world_cells = world.create_cell_group();
    auto world_graph = world.create_graph();
    Model m(world_cells);
    Predator predator(world_graph);
    m.add_agent(predator);
    Cell_group cg_gates = world.create_cell_group( world_name + "_gates" );
    Graph gates_graph(cg_gates);
    Graph gate_connections(world_cells);
    vector<Habit> world_habits = Habit::get_habits(world_graph, gates_graph, world_name);
    Reward_config rc {100,-100,-80, 1,0};
    Map map(world_cells);
    auto goal = map[{0,-7}];
    auto start = map[{0,7}];
    Habit_planner ap(world, cg_gates, start, goal, 2, rc);
    m.add_agent(ap);
    m.iterations = steps;
    Simulation c(m, {width, height}, episodes);
    c.run();
}