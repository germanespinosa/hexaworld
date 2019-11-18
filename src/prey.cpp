/*#include<gcomm.h>
#include<gmemconnector.h>
#include"definitions.h"
#include<stdlib.h>
#include<iostream>
#include <gcomm.h>

using namespace std;

int main()
{ 
    GMemConnector connector (2020, GMemConnector::mode::slave);
    GComm<State,Action> comm(connector);
    State ci;
    Action cmd;
    while (true){
        if (comm.read_object(ci)){
            cout << "received";
            std::vector<int> actions = ci.valid_actions.to_vector();
            unsigned int action = rand() % actions.size();
            cmd.iteration = ci.iteration+1;
            cmd.action =  actions[action];
            comm.write_object(cmd);
        }
    }
}
*/