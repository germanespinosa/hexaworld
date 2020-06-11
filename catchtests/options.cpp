#include<catch.h>
#include<cell_world.h>
#include<habits/simulation.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("options") {
    string world_name ("square_world");
    World world(world_name);
    world.load();
    CHECK(world.size()==225);
    Graph graph = world.create_graph();
    Map map(graph.nodes);
    Cell_group cg_gates;
    cg_gates.add(map[{0,7}]);
    cg_gates.add(map[{0,-7}]);

    Graph gates(cg_gates);
    Graph options(graph.nodes);
    vector<Graph> sws = graph.get_sub_graphs(gates, options);
    CHECK(sws.size()==1);


    for(unsigned int i=0;i<options.size();i++){
        for (unsigned int j =0;j<options[i].size();j++){
            CHECK(cg_gates.contains(options[i][j]));
        }
    }

    Graph gate_domains = !options;
    vector<Habit> habits;

    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        auto &c = gate_domains[gn];
        Graph domain(c);
        domain.add(gn);
        domain.connect(graph);
        Option option(domain, gn);
        habits.emplace_back(option);
    }
    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        CHECK(habits[i].destination.coordinates == gn.coordinates);
    }
    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        auto &domain = habits[i];
        for (unsigned int j=0;j<domain.size();j++) { //every cell in the domain must be connected
            CHECK (domain[j].size()>0);
        }
    }
}

TEST_CASE("get_options") {
    string world_name ("square_world");
    World world(world_name);
    world.load();
    CHECK(world.size()==225);
    Graph graph = world.create_graph();
    Map map(graph.nodes);
    Cell_group cg_gates;
    cg_gates.add(map[{0,7}]);
    cg_gates.add(map[{0,-7}]);

    Graph gates(cg_gates);
    vector<Option> options = Option::get_options(graph, gates);
    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        CHECK(options[i].destination.coordinates == gn.coordinates);
    }
    vector<Habit> habits;
    for (auto &o:options){
        habits.emplace_back(o);
    }
    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        auto &domain = habits[i];
        for (unsigned int j=0;j<domain.size();j++) { //every cell in the domain must be connected
            CHECK (domain[j].size()>0);
        }
    }
}

TEST_CASE("get_habits") {
    string world_name ("square_world");
    World world(world_name);
    world.load();
    CHECK(world.size()==225);
    Graph graph = world.create_graph();
    Map map(graph.nodes);
    Cell_group cg_gates;
    cg_gates.add(map[{0,7}]);
    cg_gates.add(map[{0,-7}]);
    Graph gates(cg_gates);
    vector<Habit> habits = Habit::get_habits(graph, gates);
    CHECK(habits[0].size()==198);
    CHECK(habits[1].size()==198);
    for (unsigned int i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        auto &domain = habits[i];
        for (unsigned int j=0;j<domain.size();j++) { //every cell in the domain must be connected
            CHECK (domain[j].size()>0);
        }
    }
}