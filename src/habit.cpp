#include <hexaworld.h>
#include <habit_training.h>
#include <fstream>
#include <iostream>


using namespace cell_world;
using namespace std;

bool Habit::save(const std::string& world_name) {
    std::stringstream fmt;
    fmt << "_" << sub_world_id << "_" << destination.id;
    string file_path = world_name +  fmt.str() + _extension;
    std::ofstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    for (uint32_t lc=0;lc<cells.size();lc++){
        file << values[lc].visits << " ";
        file << values[lc].lenght;
        for (double vc : values[lc].rewards) file << " " << vc ;
        file << std::endl;
    }
    return true;
}

bool Habit::load(const std::string& world_name, uint32_t options) {
    convergence = false;
    while (values.size()<cells.size())values.emplace_back(options);
    std::stringstream fmt;
    fmt << "_" << sub_world_id << "_" << destination.id;
    string file_path = world_name +  fmt.str() + _extension;
    std::ifstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    string line;
    for (uint32_t lc=0;lc<cells.size() && getline(file, line);lc++){
        istringstream ss(line);
        auto value = values [lc];
        Cell cell;
        ss >> value.visits;
        ss >> value.lenght;
        for (double & vc : value.rewards) ss >> vc;
        lc ++;
    }
    return true;
}

Habit_value::Habit_value(uint32_t options) :
        rewards(options,0),
        visits(0),
        lenght(0){
}
