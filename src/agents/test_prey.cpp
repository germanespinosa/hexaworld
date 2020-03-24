#include <test_prey.h>

using namespace cell_world;
using namespace std;

Test_prey::Test_prey(Graph &graph) :
        _cg(graph),
        _clean(false),
        Agent({"Prey",1}){
    set_color(Green);
}

const Cell &Test_prey::start_episode(uint32_t) {
    L("Cell &Test_prey::start_episode(const cell_world::State &) start");
    set_status(Action_ready);
    L("Cell &Test_prey::start_episode(const cell_world::State &) end");
    return _cg.nodes[Chance::dice(_cg.size())];
}

void Test_prey::update_state(const State &state) {
    L("Test_prey::update_state(const cell_world::State &state) start");
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

void Test_prey::end_episode(const State &) {
}