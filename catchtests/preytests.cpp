#include<catch.h>
#include "../include/prey.h"


using namespace cellworld;

TEST_CASE("right action no rep")
{
    World w();/*
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
