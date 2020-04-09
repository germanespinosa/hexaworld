#include <agents/preys/planners/action_planner.h>

using namespace cell_world;
using namespace std;

Action_planner::Action_planner( World &w, const Cell &s, const Cell &g, double time, Reward_config rc):
Planner( w , s , g , time , rc ){}

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
    switch (set.prey.result){
        case Success:
            successes[option]++;
            break;
        case Fail:
            fails[option]++;
            break;
        case Unknown:
            unknowns[option]++;
            break;
    }
    double reward = _reward_config.value(set.prey.result, set.prey.lenght);
    rewards[option] = ( rewards[option] * visits[option] + reward )/( visits[option] + 1 );
    visits[option]++;
}

void Action_planner::update_state() {
    options = _graph.get_connectors(cell());
    rewards = vector<double>(options.size(),0);
    visits = vector<uint32_t>(options.size(),0);
    fails = vector<uint32_t>(options.size(),0);
    successes = vector<uint32_t>(options.size(),0);
    unknowns = vector<uint32_t>(options.size(),0);
}

cell_world::Move Action_planner::get_best_move() {
    uint32_t option = 0;
    cout << endl << endl;
    for (uint32_t i = 0; i < options.size(); i++){
        cout << options[i] << " :" << visits[i] << " = " << rewards[i] << " - " << successes[i] << " " << unknowns[i] << " " << fails[i] << endl;
        if (rewards[i] > rewards[option]) option = i;
    }
    cout << endl << endl;
    return options[option];
}
