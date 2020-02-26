#include <cell_option.h>

using namespace std;
using namespace cell_world;

Move Policy_set::operator[](const Cell &cell) {
    return policies[_cells.find(cell)];
}

Policy_set::Policy_set(Habit &h) :
destination(h.destination),_cells(h.nodes){
    for (auto &v:h.values) policies.push_back(v.policy());
}

std::vector<Cell_option> Cell_options::operator[](const cell_world::Cell &cell) {
    return cell_options[options_graph.nodes.find(cell)];
}

Cell_options::Cell_options(const std::string &world_name) : world(world_name){
    world.load();
    world_graph = world.create_graph();
    gates = world.create_cell_group( world_name + "_gates" );
    gates_graph = Graph(gates);
    options_graph = Graph(world_graph.nodes);
    habits = Habit::get_habits(world_graph,gates_graph, options_graph);
    for (auto &h:habits) {
        policy_sets.emplace_back(h);
        gates.add(h.destination);
    }
    for (uint32_t i = 0 ; i < options_graph.nodes.size(); i++){
        auto &cell = options_graph.nodes[i];
        auto &options = options_graph[cell];
        cell_options.emplace_back();
        for (uint32_t j = 0; j < options.size() ; j++){
            auto &destination = options[j];
            auto &policies = policy_sets[gates.find(destination)];
            cell_options.back().emplace_back(cell,policies,0);
        }
    }
}

Cell_option::Cell_option(const cell_world::Cell &cell, Policy_set &policy_set, uint32_t length):
cell(cell),
policy_set(policy_set),
length(length){}
