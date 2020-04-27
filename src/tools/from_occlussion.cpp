#include <iostream>
#include <fstream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"

using namespace std;
using namespace cell_world;

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc,args);
    string world_name ("square_world");
    World world(world_name);
    world.load();
    world.name = cp[1].value();
    for (int i=0;i<world.size();i++) {
        world[i].value = 0;
        world[i].occluded = false;
    }
    Cell_group cg = world.create_cell_group();
    Map map(cg);
    string file_path = cp[1];
    std::ifstream file;
    file.open((file_path + ".csv").c_str());
    string line;
    getline(file, line);
    while (getline(file, line)) {
        istringstream ss(ReplaceAll(line,","," "));
        int16_t cx, cy;
        Cell cell;
        ss >> cx;
        ss >> cy;
        int8_t x = (int8_t)cx-7;
        int8_t y = -((int8_t)cy-7);
        world[map[{x,y}].id].occluded = true;
    }L("World::load(const std::string& ) end");
    world.save();
}