#include <hexaworld.h>
#include <fstream>
#include <iostream>
#include <habit.h>


using namespace cell_world;
using namespace std;

bool Habit::save(const std::string& world_name) {
    std::stringstream fmt;
    fmt << sub_world_id << "_" << destination.id;
    string file_path = world_name + "/" + fmt.str() + _extension;
    std::ofstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    for (uint32_t lc=0;lc<cells.size();lc++){
        file << values[lc].length << " ";
        file << values[lc].unknowns << " ";
        file << values[lc].fails << " ";
        file << values[lc].successes;
        for (uint32_t vc=0; vc< values[lc].rewards.size() ; vc++) {
            file << " " << values[lc].rewards[vc] ;
            file << " " << values[lc].visits[vc] ;
        }
        file << std::endl;
    }
    return true;
}

bool Habit::load(const std::string& world_name, uint32_t options) {
    convergence = false;
    init_values(options);
    std::stringstream fmt;
    fmt << sub_world_id << "_" << destination.id;
    string file_path = world_name + "/" +  fmt.str() + _extension;
    std::ifstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    string line;
    for (uint32_t lc=0;lc<cells.size() && getline(file, line);lc++){
        istringstream ss(line);
        Cell cell;
        ss >> values [lc].length;
        ss >> values [lc].unknowns;
        ss >> values [lc].fails;
        ss >> values [lc].successes;
        for (uint32_t vc=0; vc< values[lc].rewards.size() ; vc++) {
            ss >> values[lc].rewards[vc];
            ss >> values[lc].visits[vc];
        }
    }
    return true;
}

void Habit::add_reward(uint32_t cell_index, uint32_t action_index, Reward_config &rc, Episode_result result, uint32_t length) {
    double reward = rc.value(result, length);
    double old_visits = values[cell_index].visits[action_index];
    double old_reward = values[cell_index].rewards[action_index];
    double old_length = values[cell_index].length;
    values[cell_index].rewards[action_index]=( old_reward * old_visits + reward ) / ( old_visits + 1 );
    values[cell_index].visits[action_index]++;
    values[cell_index].length = ( old_length * old_visits + (double) length ) / ( old_visits + 1 );
}

void Habit::init_values(uint32_t options) {
    while (values.size()<cells.size())values.emplace_back(options);
}

void Habit::clear_stats() {
    for (auto &v:values) v.successes=v.fails=v.unknowns = 0;
}

void Habit::end_episode(uint32_t cell_index, Episode_result result) {
    switch(result){
        case Unknown:
            values[cell_index].unknowns++;
            break;
        case Fail:
            values[cell_index].fails++;
            break;
        case Success:
            values[cell_index].successes++;
            break;
    }
}

Habit_value::Habit_value(uint32_t options) :
        rewards(options,0),
        visits(options,0),
        length(0),
        unknowns(0),
        fails(0),
        successes(0){
}

uint32_t Habit_value::get_visits() {
    return successes+fails+unknowns;
}

uint32_t Habit_value::get_policy() {
    return 0;
}
