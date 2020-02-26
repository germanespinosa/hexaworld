#include<catch.h>
#include<cell_world.h>
#include<hexaworld.h>
#include<habit.h>
#include<iostream>
#include <cell_option.h>

using namespace cell_world;
using namespace std;

TEST_CASE("cell options") {
    string world_name ("square_world");
    Cell_options world_options(world_name);
    Cell &cell = world_options.world[14];
    CHECK(world_options.gates.size() == world_options.policy_sets.size());
    CHECK(world_options.gates[0] == world_options.policy_sets[0].destination);
    CHECK(cell.coordinates == Coordinates({7,-7}));
    CHECK(world_options.options_graph[cell].size() == 1);
    CHECK(world_options.options_graph[cell][0].coordinates == Coordinates({7,-5}));
    cout << "here: " << cell.coordinates << endl;
    cout << "here: " << world_options[cell][0].cell << endl;
    cout << "here: " << world_options.options_graph[cell] << endl;
    auto &a=world_options[cell][0].policy_set;
    for (uint32_t i = 0 ; i < a._cells.size(); i++){
        cout << "here: " << i << " " << a._cells[i] << " - policy " << a[a._cells[i]] << endl;
    }
}