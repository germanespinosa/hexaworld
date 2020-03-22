#include <planner.h>

using namespace cell_world;
using namespace std;

Planner::Planner(const World &w):
        set(w),
        _running(true),
        _thread(&Planner::_plan, this){
}

Planner::~Planner(){
    _running = false;
    _thread.join();
}

void Planner::_plan(){
    while(_running){
        _mutex.lock();
        plan();
        _mutex.unlock();
    }
}

cell_world::Move Planner::get_move() {
}
