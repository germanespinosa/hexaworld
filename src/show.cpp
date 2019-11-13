 #include <cellworld.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <sys/time.h>


using namespace cellworld;
using namespace std;

int diff_ms(timeval t1, timeval t2)
{
 return (((t1.tv_sec - t2.tv_sec) * 1000000) +
         (t1.tv_usec - t2.tv_usec))/1000;
}

int get_parameter(char *arg)
{
    std::stringstream str(arg);
    int x;  
    str >> x;
    return x;
}
struct Test_Agent : Agent {
    World &world;
    uint32_t *acum;
    struct timeval tp;
    Test_Agent(World &w) : world(w){
        gettimeofday(&tp, NULL);
        srand (tp.tv_usec);
        data.id = 1;
        data.color = Green;
        data.status=0;
        int r;
        while( world[r = rand() % world.size()].occluded);
        data.coordinates = world[r].coordinates;
        action.iteration = 0;
        action.action = {2,0};
        acum = (uint32_t *)malloc (world.size() * sizeof(uint32_t));
        for (unsigned int i=0;i<world.size();i++)acum[i]=0;
    }
    void update_state(State state) override{
        action.iteration = state.iteration;
        acum[world[data.coordinates].id]++;
        world[data.coordinates].value++;
        if (state.iteration % 1000000 == 0){
            struct timeval tn;
            gettimeofday(&tn, NULL);
            cout << "iteration: " << state.iteration << " time: "<< diff_ms(tn,tp) << endl;
            tp = tn;
        }
    }
    bool get_action() override{
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto conns = world[data.coordinates].connections;
        int r = rand() % conns.size();
        action.action = world[conns[r]].coordinates - data.coordinates;
        return true;
    }
};

int main(int argc, char *args[]){
    int w = 1024, h = 768;
    if ( argc > 1 ) w = get_parameter(args[1]);
    if ( argc > 2 ) h = get_parameter(args[2]);
    World world;
    world.load("heatmap.dat");
    Visibility vi(world);
    Test_Agent a(world);
    vector<Agent*> va;
    va.push_back(&a);
    Controller c(world,va, {w, h});
    c.run();
    /*Model m(world,va);
    for (uint32_t i = 0; i< 1000000000;i++){
        m.update();
    }*/
    world.save("heatmap.dat");

}