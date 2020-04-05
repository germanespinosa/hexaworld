#include <action_planner.h>

using namespace cell_world;
using namespace std;

Action_planner::Action_planner(const World &w, const Cell &s, const Cell &g, double time, Reward_config rc):
Planner( w , s , g , time , rc ){}

void Action_planner::plan() {
    Model &model = set.get_valid_model();
    uint32_t option = Chance::dice(options.size());// pick(Chance::combine_chances(Chance::invert_chances(visits), Chance::get_chances(rewards)));
    set.prey.set_move(options[option]);
    set.prey.successes = 0 ;
    set.prey.fails = 0;
    set.prey.unknown = 0;
    while(model.update()) set.prey.pick_random_move();
    model.end_episode();
    rewards[option] = ( rewards[option] * visits[option] + set.prey.reward )/( visits[option] + 1 );
    visits[option]++;
    successes[option]+=set.prey.successes;
    fails[option]+=set.prey.fails;
    unknowns[option]+=set.prey.unknown;
    if (set.prey.fails > 1) cout << "error" << endl;
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
    for (uint32_t i = 0; i < options.size(); i++) if (rewards[i] > rewards[option]) option = i;
    return options[option];
}
