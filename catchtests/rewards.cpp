#include<catch.h>
#include<iostream>
#include<reward_config.h>

using namespace cell_world;
using namespace std;

TEST_CASE("reward_config.value") {
    Reward_config rc {100,-100,0,.9, -1};
    CHECK(fabs(rc.value(Success,0)-100)<.01);
    CHECK(fabs(rc.value(Success,1)-89.1)<.01);
    CHECK(fabs(rc.value(Success,2)-79.38)<.01);
    CHECK(fabs(rc.value(Fail,0)+100)<.01);
    CHECK(fabs(rc.value(Fail,1)+90.9)<.01);
    CHECK(fabs(rc.value(Fail,2)+82.62)<.01);
    CHECK(fabs(rc.value(Unknown,0))<.01);
    CHECK(fabs(rc.value(Unknown,1)+.9)<.01);
    CHECK(fabs(rc.value(Unknown,2)+1.62)<.01);
}

