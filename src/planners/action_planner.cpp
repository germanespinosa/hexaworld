#include <action_planner.h>

using namespace cell_world;
using namespace std;

Action_planner::Action_planner(const World &w, const Cell &s, const Cell &g, double time):
Planner(w,s,g,time ){

}

void Action_planner::plan() {
    Model &model = set.get_valid_model();
    uint32_t option = Chance::pick(Chance::combine_chances(Chance::invert_chances(visits), Chance::get_chances(rewards)));
    set.prey.set_move(options[option]);
    for (;model.update();){
        set.prey.pick_random_move();
    }
    rewards[option] = (rewards[option] * visits[option] + set.prey.reward)/(visits[option]+1);
    visits[option]++;
}

void Action_planner::update_state() {
    options = set.prey.get_options();
    rewards = vector<double>(options.size(),0);
    visits = vector<uint32_t >(options.size(),0);
}

cell_world::Move Action_planner::get_move() {
    cout << "time: " << _clock.elapsed() << endl;
    uint32_t option = 0;
    for (uint32_t i = 1; i < options.size(); i++) if (rewards[i]>rewards[option]) option = i;
    return options[option];
}
