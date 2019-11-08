#include <model.h>
#include <ge211.h>

using namespace std;
using namespace cellworld;


void Model::_epoch( std::vector<Action> actions = {} ){
        for (unsigned int i;i<actions.size();i++) {
            if (!world[_agents[i].coordinates + actions[i].action].occluded)
                _agents[i].coordinates += actions[i].action;
        }
        _iteration++;
}

bool Model::update() // if all agents made their moves, it triggers an new poch
{
    bool new_data = false; // assumes no new actions
    
    for (unsigned int i;i<_agents.size();i++) // ask all agents to make their moves
        if (_agents[i].get_action()) 
            new_data = true; 
    
    if (new_data){ 
        for (unsigned int i;i<_agents.size();i++) // if any new moves I need to check if the iteration is over
            if (_agents[i].last_action.iteration != _iteration) // iteration is not over
                return false; 
        vector<Action> actions; 
        for (unsigned int i;i<_agents.size();i++) 
            actions.push_back(_agents[i].last_action);
        _epoch(actions); // send all actions from all agents to the epoch
        return true;
    } 
    return false;
}

Model::Model( World &world, std::vector<Agent> &agents ) : world (world), _agents (agents)
{
   _epoch();
}