#include "../include/prey.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace cellworld;
using namespace std;

void Prey::reset(uint32_t start){
    data.cell_id = _config.start;
    _iteration = 0;
    _next_action = start;
    _history.clear();
    data.status = 1;
    active = true;
}

Prey::Prey(World &w, Visibility &v, Prey_config &config)
    : _world(w)
    , _visibility(v)
    , _config(config){
    data.type = PREY_TYPE;
    data.color = config.color;
    _episodes = 0;
    _successes = 0;
}

uint32_t Prey::_get_action(std::vector<uint32_t> &options,  Prey_expected_reward *er,  int dice) {
    vector<int> indexes(options.size()); // creates an index vector for the options
    for (int i=0; i<options.size(); i++) indexes[i]=i;

    for (int i = 0; i < options.size()-1; i++) // sort the indexes of the options by expected reward descending
        for (int j = i; j>=0 && er[indexes[j]].reward < er[indexes[j+1]].reward; j--)
            swap (indexes[j+1],indexes[j]);

    int action;
    for (action = 0 ; dice > _config.action_probabilities[action] ; action++);
    double reward = er[indexes[action]].reward; // this is the expected value of the state-action pair

    int repetitions = 0;
    for (int i = 0; i < options.size(); i++) if (er[i].reward == reward) repetitions ++; //find how many times the same value appears

    int selection = dice % repetitions; //use the dice to determine which repetition to use
    for (int i = 0; i < options.size(); i++) // find the correct repetition
        if (er[i].reward == reward)
            if (!selection--) return i; //pick the correct one
}

void Prey::update_state(const State &state){
    _iteration = state.iteration;
    if (!active) return;
    uint32_t predator_cell_id = state.agents[_predator_id]->cell_id;
    if (_config.goal == data.cell_id) { //prey is in the goal
        data.status = 0;
        active = false;
        return;
    }
    if (state.agents[_predator_id]->status == 2) { // predator got the prey
        data.status = 0;
        active = false;
        return;
    }
    auto options = get_options(data.cell_id);
    int dice = rand() % (_config.action_probabilities[options.size() - 1]); // roll the dice to the right number
    bool predator_visible = _visibility.is_visible(predator_cell_id,data.cell_id);
    static int last_seen = -1;
    static int32_t last_predator_cell_id = -1;
    if (predator_visible){
        last_seen = _iteration;
        last_predator_cell_id = predator_cell_id;
    }
    Prey_state_action pa{data.cell_id, last_predator_cell_id, -1, _iteration, last_seen, predator_cell_id};
    Prey_expected_reward *er = get_expected_rewards(pa);
    pa.action = _get_action(options, er, dice);
    _history.push_back(pa);
    _next_action = options[pa.action];
}

uint32_t Prey::action_ready(){
    return _iteration;
}

uint32_t Prey::get_destination() {
    return _next_action;
}

void Prey::start_episode(const State &state) {
    reset(_config.start);
    for (unsigned int i=0 ; i<state.agents.size(); i++)
        if (state.agents[i]->type == PREDATOR_TYPE) _predator_id = i;
}

void Prey::end_episode(const State &state) {
    _episodes ++;
    double reward = _config.failure_reward;
    if (_config.goal == data.cell_id) { //prey is in the goal
        reward = _config.success_reward;
        _successes++;
    }
    for (size_t i=0 ; i < _history.size(); i++){
        save_expected_reward(_history[i], reward, _iteration);
    }
}

Prey::~Prey() {
    cout << "success rate "<< (float(_successes) / (float(_episodes)) * 100.00) << "%" << endl;
}

Prey_expected_reward_buffer::Prey_expected_reward_buffer(uint32_t size) : _size(size){
    if (size) values = (Prey_expected_reward *) calloc(_size, sizeof(Prey_expected_reward));
}

bool Prey_expected_reward_buffer::load(std::string file_name){
    ifstream rf(file_name.c_str(), ios::out | ios::binary);
    if (!rf.good()) return false;
    rf.read((char *) values, _size * sizeof(Prey_expected_reward));
    return true;
}

bool Prey_expected_reward_buffer::save(std::string file_name){
    ofstream wf(file_name.c_str(), ios::out | ios::binary);
    cout<<"saving" << endl;
    wf.write((char *) values, _size * sizeof(Prey_expected_reward));
    return true;
}

Prey_expected_reward *Prey_expected_reward_buffer::operator[](uint32_t index) {
    if (index>=_size) exit(1);
    return values + index;
}

Prey_expected_reward_buffer::~Prey_expected_reward_buffer() {
    free(values);
}
