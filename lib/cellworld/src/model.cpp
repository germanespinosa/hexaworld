#include <model.h>
#include <ge211.h>
#include <iostream>
using namespace std;
using namespace cellworld;


void Model::_epoch( std::vector<Action> actions = {} ){
        _iteration++;
        State state;
        state.iteration =_iteration;
        for (unsigned int i=0;i<actions.size();i++) {
            if (!world[_agents[i]->data.coordinates + actions[i].action].occluded)
                _agents[i]->data.coordinates += actions[i].action;
            state.agents.push_back(_agents[i]->data);
        }
        
        for (unsigned int i=0;i<actions.size();i++) {
            _agents[i]->update_state(state);
        }
}

bool Model::update() // if all agents made their moves, it triggers an new poch
{
    bool new_data = false; // assumes no new actions
    cout<<"updating!!"<<endl;
    for (unsigned int i=0;i<_agents.size();i++){ // ask all agents to make their moves
        if (_agents[i]->get_action()) {
            new_data = true; 
            cout << "data :" << _agents[i]->data.id << " " <<
            _agents[i]->data.iteration << " " <<
            (uint32_t) _agents[i]->data.status << " (" <<
            (uint16_t) _agents[i]->data.coordinates.x << "," << 
            (uint16_t)_agents[i]->data.coordinates.y << ")" <<endl;
            cout << "action :" << _agents[i]->action.iteration << " (" <<
            (uint16_t) _agents[i]->action.action.x << "," << 
            (uint16_t)_agents[i]->action.action.y << ")" <<endl;
        }
    }
    if (new_data){ 
        cout << "iteration :" << _iteration << endl;
        for (unsigned int i=0;i<_agents.size();i++){// if any new moves I need to check if the iteration is over
            if (_agents[i]->action.iteration != _iteration){ // iteration is not over
                return false; 
            }
        }
        cout << "here2!" <<endl;
        vector<Action> actions; 
        for (unsigned int i=0;i<_agents.size();i++){
            actions.push_back(_agents[i]->action);
        }
        cout << "applying " << _agents.size() << "actions" << endl;
        _epoch(actions); // send all actions from all agents to the epoch
        return true;
    } 
    return false;
}

vector<Agent_data> Model::get_agents_data(){
    vector<Agent_data> r;
    for (unsigned int i = 0; i < _agents.size(); i++)
        r.push_back(_agents[i]->data);
    return r;
}

Model::Model( World &world, std::vector<Agent*> &agents ) : 
    world (world), 
    _agents (agents)
{
   _iteration = 0;//triggers the first iteration
}