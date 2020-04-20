#include <agents/preys/planners/action_planner.h>

using namespace cell_world;
using namespace std;

Action_planner::Action_planner( World &w, const Cell &s, const Cell &g, double time, Reward_config rc, uint32_t k):
Planner( w , s , g , time , rc, k ){}

Action_planner::Action_planner( World &w, const Cell &s, const Cell &g, uint32_t i, Reward_config rc, uint32_t k):
        Planner( w , s , g , i , rc, k ){}

void Action_planner::plan() {
    Model &model = set.get_valid_model();
    uint32_t option = Chance::dice(options.size());// pick(Chance::combine_chances(Chance::invert_chances(visits), Chance::get_chances(rewards)));
    auto move = options[option];
    set.prey.set_move(move);
    while(model.update()) {
        auto o = _graph.get_connectors(set.prey.cell());
        move = o[Chance::dice(o.size())];
        set.prey.set_move(move);
    }
    model.end_episode();
    double reward = _reward_config.value(set.prey.result, set.prey.lenght);
    rewards[option] = ( rewards[option] * visits[option] + reward )/( visits[option] + 1 );
    visits[option]++;
}

void Action_planner::update_state(uint32_t &) {
    options = _graph.get_connectors(cell());
    rewards = vector<double>(options.size(),0);
    visits = vector<uint32_t>(options.size(),0);
}

cell_world::Move Action_planner::get_best_move() {
    uint32_t option = 0;
    for (uint32_t i = 0; i < options.size(); i++) if (rewards[i] > rewards[option]) option = i;
    return options[option];
}
