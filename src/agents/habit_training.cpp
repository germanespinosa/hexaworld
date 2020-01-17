//
// Created by german on 1/13/20.
//
#include <hexaworld.h>
#include <habit_training.h>
#include <fstream>
#include <iostream>


using namespace cell_world;
using namespace std;

Habit_training::Habit_training(std::vector<Habit> &habits, const Reward_config rewards, cell_world::Probabilities & probabilities)
: _habits ( habits )
, _rewards ( rewards )
, _probabilities ( probabilities )
, Agent({"Prey",2})
{
    _current_habit = 0;
}

const Cell & Habit_training::start_episode(const cell_world::State &state) {
    _predator_index = state.find("Predator");
    while (_habits[_current_habit].convergence) _current_habit = (_current_habit ++) % _habits.size();
    return _habits[_current_habit].source;
}


void Habit_training::update_state(const cell_world::State &state) {
    auto predator_cell = state.agents_data[_predator_index].cell;
    auto prey_cell = cell();
    if (prey_cell.id == _habits[_current_habit].destination.id){
        set_status(Agent_status::Finished); // success
    } else if ( _habits[_current_habit].gate_cells.contains(prey_cell)){
        set_status(Agent_status::Finished); // wrong gate
    } else if (predator_cell.id == prey_cell.id) {
        set_status(Agent_status::Finished); //eaten
    }
}

Agent_action &Habit_training::get_action() {
    auto habit = _habits[_current_habit];
    int32_t cell_index = habit.connections.cells.find(cell());
    auto options = habit.connections[cell_index].connections;
    vector <double >er(options.size());
    for ( uint32_t i=0; i<options.size();i++ ){
        int32_t connected_cell_index = habit.connections.cells.find(options[i].id);
        er[i] = habit.values[connected_cell_index];
    }
    return _action_set.actions[_probabilities.pick(er)];
}

uint32_t Habit_training::action_ready() {
    return 0;
}

void Habit_training::end_episode(const cell_world::State &) {
    _current_habit = (_current_habit++ % _habits.size());
}

void Habit::save(const std::string& world_name) {
    std::stringstream fmt;
    fmt <<"_" << source.id << "_" << destination.id;
    string file_name = world_name +  fmt.str();
    cout << "saving habit training data to " << file_name << endl;
    {
        ofstream wf((file_name + ".hab").c_str(), ios::out | ios::binary);
        char c = (char) convergence;
        wf.write((char *) &c,1);
        wf.write((char *) &values[0], values.size() * sizeof(double));
        wf.write((char *) &policies[0], policies.size() * sizeof(uint32_t));
        wf.write((char *) &visits[0], visits.size() * sizeof(uint32_t));
    }
}

Habit::Habit(const Cell &source, const Cell &destination)
: source(source), destination(destination), convergence(false)
{}

bool Habit::load(const std::string& world_name) {
    while (values.size()<connections.cells.size())values.push_back(0);
    while (policies.size()<connections.cells.size())policies.push_back(0);
    while (visits.size()<connections.cells.size())visits.push_back(0);
    std::stringstream fmt;
    fmt <<"_" << source.id << "_" << destination.id;
    string file_name = world_name +  fmt.str();
    {
        ifstream rf(file_name.c_str(), ios::out | ios::binary);
        if (!rf.good()) return false;
        char c = 0;
        rf.read((char *) &c, 1);
        convergence = c;
        rf.read((char *) &values[0], values.size() * sizeof(double));
        rf.read((char *) &policies[0], policies.size() * sizeof(uint32_t));
        rf.read((char *) &visits[0], visits.size() * sizeof(uint32_t));
    }
    return true;
}

void Habit::compute_polices() {
    convergence = true;
    for ( uint32_t i = 0; i < connections.cells.size(); i++){
        auto c = connections[connections.cells[i]].connections;
        uint32_t best = connections.cells.find(c[0].id);
        for ( uint32_t j = 1; j< c.size() ; j++){
            uint32_t candidate = connections.cells.find(c[0].id);
            if (values[candidate] > values[best]) best = candidate;
        }
        if ( policies[i] != best) {
            policies[i] = best;
            convergence = false;
        }
    }
}

