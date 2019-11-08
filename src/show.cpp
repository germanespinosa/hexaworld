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
int main(int argc, char *args[]){
    int w = 1024, h = 768;
    if ( argc > 1 ) w = get_parameter(args[1]);
    if ( argc > 2 ) h = get_parameter(args[2]);
    World world;
    vector<Agent> a;
    world.load("output.dat");
    Controller c(world,a, {w, h});
    c.run();
}