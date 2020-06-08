#include <fstream>
#include <iostream>
#include <habits/habit.h>

using namespace cell_world;
using namespace std;

bool Habit::save(const std::string& world_name) {
    string file_path = world_name + "/" + _file_name();
    std::ofstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    for (auto &v:values){
        file << v.length << " ";
        file << v.unknowns << " ";
        file << v.fails << " ";
        file << v.successes << " ";
        file << v.visits;
        for (auto &a:v.actions) {
            file << " " << a.visits ;
            file << " " << a.reward ;
        }
        file << std::endl;
    }
    return true;
}
bool Habit::load(const std::string& world_name) {
    string file_path = world_name + "/" +  _file_name();
    std::ifstream file;
    file.open(file_path.c_str());
    if (!file.good()) {
        return false;
        cout << "failed to load" << endl;
    }
    string line;
    for (auto &v:values){
        if (!getline(file, line)) return false;
        istringstream ss(line);
        ss >> v.length;
        ss >> v.unknowns;
        ss >> v.fails;
        ss >> v.successes;
        ss >> v.visits;
        for (auto &a:v.actions) {
            ss >> a.visits;
            ss >> a.reward;
        }
    }
    return true;
}

void Habit::add_reward(unsigned int cell_index, unsigned int action_index, Reward_config &rc, Episode_result result, unsigned int length) {
    auto &value = values[cell_index];
    auto &action = value.actions[action_index];
    double reward = rc.value(result, length);
    double old_visits = action.visits;
    double old_reward = action.reward;
    double old_length = value.length;
    action.reward =( old_reward * old_visits + reward ) / ( old_visits + 1 );
    action.visits++;
    value.length = ( old_length * old_visits + (double) length ) / ( old_visits + 1 );
    //cout << "reward for " << nodes[cell_index].coordinates << " action " << action.move << " : " << reward  << " " << length << endl;
}

void Habit::end_episode(unsigned int cell_index, Episode_result result) {
    values[cell_index].visits++;
    switch(result){
        case Unknown:
            values[cell_index].unknowns++;
            break;
        case Fail:
            values[cell_index].fails++;
            break;
        case Success:
            values[cell_index].successes++;
            break;
    }
}

Habit::Habit(Option option)
:Option(option)
{
    for (unsigned int i = 0; i < option.size() ; i++){
        Habit_value hv;
        for (unsigned int j = 0; j < option[i].size() ; j++){
            Move a( option[i][j].coordinates - option.nodes[i].coordinates );
            hv.actions.emplace_back(a,0,0);
        }
        values.push_back(hv);
    }
}

string Habit::_file_name() {
    std::stringstream fmt;
    Coordinates d = destination.coordinates;
    fmt << "habit_" << (d.x<0?'N':'P') << abs(d.x) << '_' << (d.y<0?'N':'P') << abs(d.y)  <<_extension;
    return fmt.str();
}

vector<Habit> Habit::get_habits(Graph &world_graph, Graph &gates_graph) {
    Graph option_graph(world_graph.nodes);
    return get_habits(world_graph, gates_graph, option_graph);
}

vector<Habit> Habit::get_habits(Graph &world_graph, Graph &gates_graph, const string &world_name) {
    Graph option_graph(world_graph.nodes);
    return get_habits(world_graph, gates_graph, option_graph, world_name);
}

vector<Habit> Habit::get_habits(Graph &world_graph, Cell_group &gates) {
    Graph gates_graph(gates);
    return get_habits(world_graph,gates_graph);
}

std::vector<Habit> Habit::get_habits(cell_world::Graph &world_graph, cell_world::Cell_group &gates, const std::string &world_name) {
    Graph gates_graph(gates);
    return get_habits(world_graph,gates_graph,world_name );
}

std::vector<Habit> Habit::get_habits(cell_world::Graph &world_graph, cell_world::Graph &gates_graph, cell_world::Graph &option_graph) {
    vector<Option> options = Option::get_options(world_graph, gates_graph,option_graph);
    vector<Habit> habits;
    for (auto &o:options) habits.emplace_back(o);
    return habits;
}

std::vector<Habit>
Habit::get_habits(cell_world::Graph &world_graph, cell_world::Graph &gates_graph, cell_world::Graph &option_graph, const std::string &world_name) {
    auto habits = Habit::get_habits(world_graph,gates_graph, option_graph);
    for (auto &h:habits) h.load(world_name);
    return habits;
}

cell_world::Move Habit::policy(const cell_world::Cell &cell) {
    unsigned int i = nodes.find(cell);
    return values[i].policy();
}

Move Habit_value::policy() {
    vector<double> values (actions.size());
    for (unsigned int i=0;i<values.size();i++) values[i] = actions[i].reward;
    return actions[Chance::pick_best(1,values)].move;
}

Habit_action::Habit_action(Move a, unsigned int v, double r) :
move(a),
visits(v),
reward(r){}

Habit_set::Habit_set(const cell_world::World &world, const cell_world::Cell_group &gates){
    Graph world_graph = world.create_graph();
    Graph gates_graph(gates);
    habits = Habit::get_habits(world_graph, gates_graph, options, world.name);
    for (auto &h:habits)destinations.add(h.destination);
}

std::vector<std::reference_wrapper<Habit>> Habit_set::operator[](const cell_world::Cell &cell) {
    std::vector<std::reference_wrapper<Habit>> r;
    auto o = options[cell];
    for (unsigned int i = 0;i<o.size();i++){
        unsigned int index = destinations.find(o[i]);
        r.emplace_back(habits[index]);
    }
    return r;
}
