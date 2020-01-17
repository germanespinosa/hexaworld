#include <hexaworld.h>
#include <fstream>

using namespace cell_world;
using namespace std;

bool Action_set::load(const std::string& name) {
    string file_name = name + _extension;
    actions.clear();
    std::ifstream file;
    file.open(file_name.c_str());
    if (!file.good()) return false;
    string line;
    while (getline(file, line)){
        std::stringstream fmt;
        fmt << "_" << displacements.size();
        string action_name = name + fmt.str();
        Agent_action aa;
        aa.load(action_name);
        actions.push_back(aa);
        istringstream ss(line);
        double cx,cy;
        ss >> cx;
        ss >> cy;
        displacements.push_back({cx,cy});
    }
    return true;
}

bool Action_set::save(const std::string& name) {
    for (uint32_t i = 0; i<actions.size() ;i++) {
        std::stringstream fmt;
        fmt << "_" << i;
        string action_name = name + fmt.str();
        actions[i].save(action_name);
    }
    return true;
}
