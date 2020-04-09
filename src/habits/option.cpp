#include<habits/option.h>

using namespace std;
using namespace cell_world;

vector<Option> Option::get_options(Graph &graph, Graph &gates) {
    Graph options_graph(graph.nodes);
    return Option::get_options(graph,gates,options_graph);
}

vector<Option> Option::get_options(Graph &graph, cell_world::Cell_group &gates) {
    Graph gates_graph(gates);
    return Option::get_options(graph, gates_graph);
}

Option::Option(Graph &g, const cell_world::Cell &d) :
destination(d), Graph(g) {
    remove(d);
}

std::vector<Option> Option::get_options(cell_world::Graph &graph, cell_world::Graph &gates, cell_world::Graph &options_graph) {
    vector<Graph> sub_graphs = graph.get_sub_graphs(gates, options_graph);
    Graph gate_domains = !options_graph;
    vector<Option> options;
    for (uint32_t i=0;i<gates.size();i++){
        auto &gn = gates.nodes[i];
        Graph domain(gate_domains[gn]); // all the cells connected to the destination
        domain.add(gn); // adds the destination
        domain.connect(graph); //connects the cells using the original graph pattern
        options.emplace_back(domain, gn);
    }
    return options;
}