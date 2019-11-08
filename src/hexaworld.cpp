#include <iostream>
#include <fstream>
#include <model.h>
#include <view.h>
#include <controller.h>
#include <gcomm.h>
#include <gmemconnector.h>

using namespace ge211;
using namespace std;

int main()
{ 
/*    ifstream in_file;
    in_file.open("setup.dat", ios::in);
    if (in_file.fail()) {
        cout << "Could not open input file.  Program terminating.\n\n";
        return 9;
    }
    vector<Coordinates> occlussions;
    int x, y, t = 0;
    Coordinates start;
    Coordinates goal;
    Coordinates predator;
    while (t!=4){
        in_file >> x;
        in_file >> y;
        in_file >> t;
        Coordinates coordinates{(int8_t)x,(int8_t)y};
        occlussions.push_back(coordinates);
    }*/
/*    GMemConnector prey_connector (8020, GMemConnector::mode::master);
    GComm<Action, State> prey_comm(prey_connector);
    GMemConnector predator_connector (8030, GMemConnector::mode::master);
    GComm<Action, State> predator_comm(predator_connector);
    Hexaworld{occlussions, start, goal, predator, prey_comm, predator_comm}.run(); */
}
