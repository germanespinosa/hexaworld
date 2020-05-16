#include <cell_world.h>
#include <iostream>
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/test_prey.h"

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
    auto world_cells = world.create_cell_group();
    auto world_graph = world.create_graph();
    Paths paths = world.create_paths(world_name, cell_world::Paths::Path_type::shortest);
    Model m(world_cells);
    Predator predator(world_graph,m.visibility, paths);
    m.add_agent(predator);
    Cell_group cg_gates = world.create_cell_group( world_name + "_gates" );
    Graph gates_graph(cg_gates);
    Graph gate_connections(world_cells);
    Reward_config rc {100,-100,-80, 1,0};
    Map map(world_cells);
    Test_prey tp (world_graph);
    m.add_agent(tp);
    m.iterations = steps;
    Simulation c(m, {width, height}, episodes);
    c.run();
    cout << m.history;
}