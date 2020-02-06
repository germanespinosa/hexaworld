#include <action_set.h>
#include <iostream>

using namespace cell_world;
using namespace std;

Action_set::Action_set(cell_world::Cell_group &cells, cell_world::Connection_pattern cp, cell_world::Chance &prob) {
    Map map(cells);
    Cell_group cg;
    for(auto p:cp.pattern) cg.add(map[p]);
    for(auto p:cp.pattern) _add_action(map[p].location,cg,prob);
}

void Action_set::_add_action(Location source, cell_world::Cell_group &destinations, cell_world::Chance &prob) {
    Agent_action aa;
    aa.probabilities = prob;
    auto indexes = new_index(destinations.size()); // creates an index vector for the options

    for (uint32_t i = 1; i < indexes.size(); i++)
        for (uint32_t j = i; j > 0 &&
                             destinations[indexes[j - 1]].location.dist(source) > destinations[indexes[j]].location.dist(source); j--)
            swap(indexes[j - 1], indexes[j]);

    Location displacement{0,0};
    for (uint32_t i = 0; i < indexes.size(); i++){
        aa.destinations.pattern.push_back(destinations[indexes[i]].coordinates);
        displacement += destinations[indexes[i]].location * prob.probability(i);
    }
    _actions.push_back(aa);
    _displacements.push_back(displacement);
}

uint32_t Action_set::get_best(cell_world::Location l) {
    uint32_t min = 0;
    for (uint32_t i = 1; i<_actions.size() ; i++) if (_displacements[min].dist(l) > _displacements[i].dist(l)) min = i;
    return min;
}

cell_world::Agent_action &Action_set::operator[](uint32_t i) {
    return _actions[i];
}

uint32_t Action_set::size() {
    return _actions.size();
}
