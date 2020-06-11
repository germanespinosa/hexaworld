#include <cell_world.h>
#include <iostream>
#include "utils.h"
#include "agents/predator.h"
#include "agents/preys/planners/point_planner.h"
//11 60 90
using namespace cell_world;
using namespace std;


int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    Reward_config rc {cp["-success_reward"].double_value(100),
                      cp["-failure_reward"].double_value(0),
                      cp["-default_reward"].double_value(0),
                      cp["-discount"].double_value(1),
                      cp["-step_cost"].double_value(0)};

    unsigned int iterations = cp["-iterations"].int_value(1);
    cp[1].check_present().check_file_exist(".world");
    Paths::Path_type path_type = Paths::Path_type::euclidean;
    if (cp["-Navigation_strategy"].value("shortest") == "shortest"){
        path_type=Paths::Path_type::shortest;
    }
    bool show = cp["-show"].present();
    int64_t p_seed = cp["-seed"].default_value(-1).check_range(-1,65535).int_value();
    uint16_t steps = cp["-steps"].int_value(80);
    unsigned int episodes = cp["-episodes"].int_value(1);
    unsigned int k = cp["-particles"].int_value(10000);
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    unsigned int planning_roll_outs = cp["-planning_roll_outs"].int_value(5000);
    unsigned int planning_time = cp["-planning_time"].double_value(1);

    Planning_unit pu = Planning_unit::roll_outs;
    unsigned int pa = planning_roll_outs;
    if (cp["-planning_time"].present()) {
        pu = Planning_unit::milliseconds;
        pa = planning_time;
    }

    Planning_strategy ps=Planning_strategy::micro_habits;
    if (cp["-plan_over_shortest_path"].present()) ps = Planning_strategy::shortest_path;

    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    auto world_cells = world.create_cell_group();
    auto world_graph = world.create_graph();
    Model m(world_cells, steps);
    Paths paths = world.create_paths(world_name, path_type);

    Map map(world_cells);

    auto goal = map[{0,-7}];
    if (cp["-goal_x"].present() && cp["-goal_y"].present()) {
        Coordinates coo ;
        coo.x = (int8_t) (cp["-goal_x"].int_value());
        coo.y = (int8_t) (cp["-goal_y"].int_value());
        goal = map[coo];
    }

    Predator predator(world_graph, m.visibility, paths, rc, goal);
    if (cp["-predator_x"].present() && cp["-predator_y"].present()){
        Coordinates coo ;
        coo.x = (int8_t) (cp["-predator_x"].int_value());
        coo.y = (int8_t) (cp["-predator_y"].int_value());
        predator.set_fixed_start(map[coo]);
    }
    int pr = cp["-predator_randomness"].default_value(25).check_range(0,100).int_value();
    predator.set_randomness(pr);
    Cell_group cg_gates = world.create_cell_group( world_name + "_gates" );
    Graph gates_graph(cg_gates);
    Graph gate_connections(world_cells);
    vector<Habit> world_habits = Habit::get_habits(world_graph, gates_graph, world_name);

    auto start = map[{0,7}];
    if (cp["-prey_x"].present() && cp["-prey_y"].present()) {
        Coordinates coo ;
        coo.x = (int8_t) (cp["-prey_x"].int_value());
        coo.y = (int8_t) (cp["-prey_y"].int_value());
        start = map[coo];
    }

    Point_planner api(world, cg_gates, start, goal, ps, pu, pa, rc, k, paths, steps);

    m.add_agent(api);
    m.add_agent(predator);

    if (show) {
        Simulation c(m, {width, height}, episodes);
        c.run();
    }
    else {
        cout << "[";
        for (unsigned int iteration = 0;iteration < iterations;iteration++) {
            if (iteration) cout << ",";
            if (p_seed>=0) p_seed++;
            set_seed(p_seed);
            m.start_episode();
            while (m.update());
            m.end_episode();
            cout << m.log;
        }
        cout << "]";
    }
}