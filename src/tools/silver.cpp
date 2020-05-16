#include <cell_world.h>
#include <iostream>
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/planners/action_planner.h"
#include "agents/preys/test_prey.h"

using namespace cell_world;
using namespace std;

int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    cp[1].check_present().check_file_exist(".world");
    bool show = cp["-show"].present();
    int64_t p_seed = cp["-seed"].default_value(-1).check_range(-1,65535).int_value();
    uint16_t steps = cp["-steps"].int_value(80);
    uint32_t episodes = cp["-episodes"].int_value(1);
    uint32_t k = cp["-k"].int_value(10000);
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    uint32_t planning_iterations = cp["-pi"].int_value(5000);
    float time = cp["-pt"].double_value(1);
    set_seed(p_seed);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    auto world_cells = world.create_cell_group();
    auto world_graph = world.create_graph();
    Model m(world_cells);
    Paths paths = world.create_paths(world_name, cell_world::Paths::Path_type::shortest);
    Predator predator(world_graph, m.visibility, paths);
    m.add_agent(predator);
    Cell_group cg_gates = world.create_cell_group( world_name + "_gates" );
    Graph gates_graph(cg_gates);
    Graph gate_connections(world_cells);
    Reward_config rc {100,-100,-100, 1,0};
    Map map(world_cells);
    auto goal = map[{0,-7}];
    auto start = map[{0,7}];
    Action_planner apt(world, start,goal,time, rc,k,paths);
    Action_planner api(world, start,goal, planning_iterations, rc,k,paths);
    if (cp["-pt"].present()) m.add_agent(apt);
    else m.add_agent(api);
    m.iterations = steps;
    Simulation c(m, {width, height}, episodes);
    if (show)
        c.run();
    else
        c.run_silent(false);
}