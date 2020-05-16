#include <iostream>
#include <fstream>
#include <ge211.h>
#include <cell_world.h>
#include "utils.h"

using namespace std;
using namespace cell_world;


vector<Connection_pattern> patterns(const string &s) {
    vector<Connection_pattern> ps;
    stringstream s_stream(s);
    while (s_stream.good()) {
        string substr;
        getline(s_stream, substr, '/'); //get first string delimited by comma
        Connection_pattern cp;
        cp=substr;
        ps.push_back(cp);
    }
    return ps;
}


struct Handler : Viewer_handler {
};

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc,args);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    Paths euclidean = world.create_paths(Paths::Path_type::euclidean);
    Paths shortest = world.create_paths(Paths::Path_type::shortest);
    euclidean.save();
    shortest.save();
}