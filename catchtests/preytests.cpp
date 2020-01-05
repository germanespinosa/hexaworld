#include<catch.h>
#include "../include/prey.h"


using namespace cellworld;

TEST_CASE("right action no rep")
{
    World w("test");
    Cell c1(0,{0,1},{1,1},0,false);
    Cell c2(1,{1,1},{1,1},0,false);
    Cell c3(2,{2,1},{1,1},0,false);
    Cell c4(3,{3,1},{1,1},0,false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Connections wc;
    w.get_connections(wc,{{-1,0},{1,0}});
    wc.process_eigen_centrality();
    CHECK(wc[0].size()==1);
    CHECK(wc[0][0]==1);
    CHECK(wc[1].size()==2);
    CHECK(wc[1][0]==0);
    CHECK(wc[1][1]==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[2][0]==1);
    CHECK(wc[2][1]==3);
    CHECK(wc[3].size()==1);
    CHECK(wc[3][0]==2);/*
    CHECK(abs(wc.eigen_centrality[0] - 0.371748)<.001);
    CHECK(abs(wc.eigen_centrality[1] - 0.601501)<.001);
    CHECK(abs(wc.eigen_centrality[2] - 0.601501)<.001);
    CHECK(abs(wc.eigen_centrality[3] - 0.371748)<.001);*/
        /*
    Expected_reward er[] = {{1,10},{1,20},{1,5},{1,1}};
    int probability[] = {70,80,90,100};
    CHECK (Prey::get_action(4,er,5,probability) == 1);
    CHECK (Prey::get_action(4,er,15,probability) == 1);
    CHECK (Prey::get_action(4,er,25,probability) == 1);
    CHECK (Prey::get_action(4,er,75,probability) == 0);
    CHECK (Prey::get_action(4,er,85,probability) == 2);
    CHECK (Prey::get_action(4,er,95,probability) == 3);*/
}

TEST_CASE("right action rep")
{
    /*Expected_reward er[] = {{1,20},{1,20},{1,20},{1,20}};
    int probability[] = {70,80,90,100};
    CHECK (Prey::get_action(4,er,0,probability) == 0);
    CHECK (Prey::get_action(4,er,5,probability) == 1);
    CHECK (Prey::get_action(4,er,6,probability) == 2);
    CHECK (Prey::get_action(4,er,7,probability) == 3);*/
}


