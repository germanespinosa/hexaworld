#include<catch.h>
#include<cell_world.h>
#include <hexaworld.h>
#include <core.h>
#include<iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("reward_config.value") {
    Reward_config rc {100,-100,0,.9, -1};
    CHECK(rc.value(Success,0)==100);
    CHECK(rc.value(Success,1)==89);
    CHECK(rc.value(Success,2)==79);
    CHECK(rc.value(Fail,0)==-100);
    CHECK(rc.value(Fail,1)==-91);
    CHECK(rc.value(Fail,2)==-83);
    CHECK(rc.value(Unknown,0)==0);
    CHECK(rc.value(Unknown,1)==-1);
    CHECK(rc.value(Unknown,2)==-2);
}

