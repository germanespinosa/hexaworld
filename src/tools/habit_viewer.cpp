#include <cell_world.h>
#include <iostream>
#include <stdlib.h>
#include "utils.h"
#include "predator.h"
#include "test_prey.h"
#include "habit_training.h"
#include "hexaworld.h"
#include <sys/stat.h>

using namespace cell_world;
using namespace std;

struct Handler : Viewer_handler{
    Handler(World &world, Cell_group &gates):
    _world (world),
    _gates (gates){
        auto world_graph = world.create_graph();
        Graph gates_graph(gates);
        _habits = Habit::get_habits(world_graph, gates_graph, world.name);
        Cell_group_view destination {Cell_group(),Blue,true};
        cells_view.emplace_back(destination );
        for (uint32_t i=0;i<gates.size();i++){
            world[gates[i].id].icon = Bridge_icon;
        }
    }
    void on_key_up(Key key) override {
        if (key == Key::code('q') ) {
            exit(0);
        }
        if (key == Key::up()){
            _current_habit++;
            if (_current_habit ==_habits.size()) _current_habit = 0;
        }
        if (key == Key::down()) {
            if (_current_habit == 0) _current_habit = _habits.size();
            _current_habit--;
        }
        if( _current_habit == Not_found ) {
            message = to_string(_habits.size()) + " habits";
        } else {
            message = "habit "  + to_string(_current_habit);
        }
        cells_view[0].cells.clear();
        cells_view[0].cells.add(_habits[_current_habit].destination);
        for(uint32_t i=0;i<_world.size();i++) {
            _world[i].value = 0;
            if ( !_gates.contains(_world[i]) ) _world[i].icon = No_icon;
        }
        _habits[_current_habit].load(_world.name);
        double  max = 1;
        /*for(uint32_t i=0;i<_habits[_current_habit].values.size();i++){
            if (_habits[_current_habit].values[i].visits>max) max = _habits[_current_habit].values[i].visits;
        }*/
        for(uint32_t i=0;i<_habits[_current_habit].values.size();i++) {
            auto &cell = _habits[_current_habit].nodes[i];
            if ( !_gates.contains(cell) ) {
                _world[cell.id].icon = Arrow_icon;
                _world[cell.id].direction = _habits[_current_habit].values[i].policy();
            }
            //_world[cell.id].value = ((double) _habits[_current_habit].values[i].visits )/max;
        }
    }
private:
    int32_t _current_habit = Not_found;
    World &_world;
    vector<Habit> _habits;
    Cell_group &_gates;
};

int main(int argc, char *args[]){
    Cmd_parameters cp(argc,args);
    cp[1].check_present().check_file_exist(".world");
    int64_t p_seed = cp["-seed"].default_value(-1).check_range(-1,65535).int_value();
    uint16_t steps = cp["-steps"].int_value(1000);
    uint32_t episodes = cp["-episodes"].int_value(1);
    int width = cp["-width"].int_value(1024);
    int height = cp["-height"].int_value(768);
    set_seed(p_seed);
    string world_name (cp[1].value());
    World world(world_name);
    world.load();
    Cell_group cg_gates = world.create_cell_group( world_name + "_gates" );
    auto world_cells = world.create_cell_group();
    Handler handler( world, cg_gates);
    Viewer viewer(world_cells, handler);
    viewer.run();
}