#include <agents/preys/test_prey.h>
#include <chrono>
#include <thread>

using namespace cell_world;
using namespace std;

Test_prey::Test_prey(Graph &graph) :
        _cg(graph),
        _map(_cg.nodes),
        _clean(false){
}

const Cell &Test_prey::start(uint32_t) {
    set_goal(_map[{0,-7}]);
    return _cg.nodes[Chance::dice(_cg.size())];
}

void Test_prey::update(const State &state) {
    set_status(Action_ready);
}

Move Test_prey::get_move() {
    Connection_pattern cp = Connection_pattern::get_pattern(cell(),_cg[cell()]);
    return cp[Chance::dice(cp.size())];
}
