#include <planner.h>

using namespace cell_world;
using namespace std;

Planner::Planner(const World &w):
        set(w),
        _thread(&Planner::_plan, this){
}

Planner::~Planner(){
    _running = false;
    _thread.join();
}

void Planner::_plan(){
    _running = true;
    while(_running){
        if (planning) {
            plan();
        }
    }
}

cell_world::Move Planner::get_move() {
}
