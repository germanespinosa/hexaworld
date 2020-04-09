#include <agents/preys/test_prey.h>
#include <chrono>
#include <thread>

using namespace cell_world;
using namespace std;

Test_prey::Test_prey(Graph &graph) :
        _cg(graph),
        _clean(false){
}

const Cell &Test_prey::start(uint32_t) {
    L("Cell &Test_prey::start_episode(const cell_world::State &) start");
    set_status(Action_ready);
    L("Cell &Test_prey::start_episode(const cell_world::State &) end");
    return _cg.nodes[Chance::dice(_cg.size())];
}

void Test_prey::update(const State &state) {
    L("Test_prey::update_state(const cell_world::State &state) start");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    set_status(Action_ready);
    if (!state.agents_data.empty()) {
        double d = cell().location.dist(state.agents_data[0].cell.location);
        if (d<3) _clean = true;
    }
    L("Test_prey::update_state(const cell_world::State &state) end");
}

Move Test_prey::get_move() {
    if (_clean) {
        _clean = false;
        return _cg.nodes[Chance::dice(_cg.size())].coordinates - cell().coordinates;
    } else {
        Connection_pattern cp = Connection_pattern::get_pattern(cell(),_cg[cell()]);
        auto move = cp[Chance::dice(cp.size())];
        return move;
    }
}