#pragma once
#include <cell_world.h>

struct Planner_history{
    void add_record(uint32_t, const cell_world::Cell &);
    const cell_world::Cell &last_record();
    bool contains (uint32_t);
    const cell_world::Cell &operator[](uint32_t);
    void clear();
    uint32_t last_update;
    uint32_t record_count = 0;
private:
    std::vector<int32_t> _data_points;
    cell_world::Cell_group _cells;
};
