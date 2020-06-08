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
    vector<Connection_pattern> ps = patterns (cp[2].value());
    Connection_pattern extra_pois;
    extra_pois = cp[3].value();
    World world(world_name);
    world.load();
    for (unsigned int i = 0; i < world.size(); i++) world[i].value = 0;
    Cell_group group = world.create_cell_group();
    vector<double> connectivity (group.size(),0);
    vector<double> derivative (group.size(),0);
    Graph graph = world.create_graph();
    Map map(group);

    for (unsigned int i = 0; i < world.size(); i++)
        connectivity[i] = ((double)graph[world[i]].size());

    Coordinates left{-1,0};
    Coordinates right{1,0};
    Coordinates up{0,-1};
    Coordinates down{0,1};
    double max = 0;
    for (unsigned int i = 0; i < world.size(); i++) {
        if (world[i].occluded) continue;
        auto c = world[i].coordinates;
        double local = connectivity[i];
        double v = 1;
        double lv = 0;
        for (auto p : ps){
            for (auto cc: p.get_candidates(c)) {
                lv = 0;
                if (map.find(cc) != Not_found) {
                    lv += abs(connectivity[map[cc].id] - local);
                } else {
                    lv += local;
                }
            }
            v*=lv;
        }
        if ( v>max ) max = v;
        derivative[i] = v;
    }

    for (auto c:extra_pois.pattern){
        derivative[map[c].id] = 1;
        for (unsigned int i=0;i<world.size();i++)
            if(!world[i].occluded && graph.get_shortest_path(map[c],world[i]).size() == 0) world[i].occluded = true;
    }

    Cell_group pois;

    for (unsigned int i = 0; i < world.size(); i++) {
        if (!world[i].occluded && derivative[i] > 0) {
            world[i].value =  1 ;
            pois.add(world[i]);
        } else {
            world[i].value =  0 ;
        }
    }

    pois.save(world.name + "_gates");

    if (cp["-show"].present()) {
        Handler h;
        Viewer viewer(group, h);
        viewer.run();
    }
    for (unsigned int i=0;i<world.size();i++) world[i].value = 0;
    world.save();
}