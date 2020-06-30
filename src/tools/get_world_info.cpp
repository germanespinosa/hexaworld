#include <iostream>
#include <cell_world.h>
#include "utils.h"

using namespace std;
using namespace cell_world;

int main (int argc, char *args[]) {
    Cmd_parameters cp(argc,args);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    Cell_group cg = world.create_cell_group();
    Graph visibility = Visibility::create_graph(cg);
    std::vector<int> vision_field;
    for (unsigned int i = 0; i<visibility.nodes.size(); i++){
        vision_field.push_back(visibility[i].size());
    }
    std::vector<int> hist = histogram(vision_field);
    set_output_format(cell_world::Output_format::Object);
    cout << "{\"world\": "  << world << ", \"complexity\": " << entropy(hist) << ", \"occlusions\": [";
    bool first = true;
    int smallestX = cg[0].coordinates.x, smallestY = cg[0].coordinates.y, biggestX = cg[0].coordinates.x, biggestY = cg[0].coordinates.y;
    for (unsigned int i=0;i<cg.size();i++){
        if (biggestX<cg[i].coordinates.x) biggestX=cg[i].coordinates.x;
        if (smallestX>cg[i].coordinates.x) smallestY=cg[i].coordinates.x;
        if (biggestY<cg[i].coordinates.y) biggestY=cg[i].coordinates.y;
        if (smallestY>cg[i].coordinates.y) smallestY=cg[i].coordinates.y;
        if (cg[i].occluded){
            if (first) first = false;
            else cout << ",";
            cout << "[" << (int)cg[i].coordinates.x << "," << (int)cg[i].coordinates.y << "]";
        }
    }
    cout << "], \"visibility\": " << visibility << ", \"coordinates\": [[" << smallestX << "," << smallestY << "],[" << biggestX << "," <<  biggestY << "]]}";

}