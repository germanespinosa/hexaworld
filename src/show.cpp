 #include <cellworld.h>
#include <iostream>

using namespace cellworld;
using namespace std;

int get_parameter(char *arg)
{
    std::stringstream str(arg);
    int x;  
    str >> x;
    return x;
}
struct Test_Agent : Agent {
    World &world;
    Test_Agent(World &w) : world(w){
        data.id = 1;
        data.color =6;
        data.status=0;
        data.coordinates = {-20,0};
        action.iteration = 0;
        action.action = {2,0};
    }
    void update_state(State state) override{
        action.iteration = state.iteration;
    }
    bool get_action() override{
        if (data.coordinates.x >= 20) return false;
        action.action = world[world[data.coordinates].connections[1]].coordinates - data.coordinates;
        return true;
    }
};

int main(int argc, char *args[]){
    int w = 1024, h = 768;
    if ( argc > 1 ) w = get_parameter(args[1]);
    if ( argc > 2 ) h = get_parameter(args[2]);
    World world;
    world.load("output.dat");
    Test_Agent a(world);
    vector<Agent*> va;
    va.push_back(&a);

    Controller c(world,va, {w, h});
    c.run();

/*    Model m(world,va);
    m.update() ;
    cout << a.data.coordinates.x<< endl;
    m.update() ;
    cout << a.data.coordinates.x<< endl;
    m.update() ;
    cout << a.data.coordinates.x<< endl;*/

}