#include <agents/preys/planners/action_planner.h>

using namespace cell_world;
using namespace std;

Action_planner::Action_planner( World &w, const Cell &s, const Cell &g, Planning_unit pu, unsigned int i, Reward_config rc, unsigned int k, cell_world::Paths &p, unsigned int iterations):
        Planner( w , s , g , pu, i , rc, k, p, iterations ){}

void Action_planner::plan() {
    Model &model = set.get_valid_model();
    unsigned int option = Chance::dice(options.size());// pick(Chance::combine_chances(Chance::invert_chances(visits), Chance::get_chances(rewards)));
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

void Action_planner::update_state(unsigned int &) {
    options = _graph.get_connectors(cell());
    rewards = vector<double>(options.size(),0);
    visits = vector<unsigned int>(options.size(),0);
}

cell_world::Move Action_planner::get_best_move() {
    unsigned int option = 0;
    for (unsigned int i = 0; i < options.size(); i++) if (rewards[i] > rewards[option]) option = i;
    return options[option];
}
