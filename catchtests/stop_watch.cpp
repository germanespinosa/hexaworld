#include<catch.h>
#include<utils.h>

using namespace std;

TEST_CASE("stopwach") {
    Stop_watch sw;
    Stop_watch sw2;
    sw2.reset();
    sw.reset();
    while (!sw.time_out(.5));
    CHECK(sw.elapsed()>=.5);
    sw.reset();
    while (!sw.time_out(.5));
    CHECK(sw.elapsed()>.5);
    CHECK(sw2.elapsed()>1);
}

TEST_CASE("stopwach_int") {
    Stop_watch sw;
    Stop_watch sw2;
    uint32_t t = 500;
    sw2.reset();
    sw.reset();
    while (!sw.time_out(t));
    CHECK(sw.elapsed()>=.5);
    sw.reset();
    while (!sw.time_out(t));
    CHECK(sw.elapsed()>.5);
    CHECK(sw2.elapsed()>1);
}

