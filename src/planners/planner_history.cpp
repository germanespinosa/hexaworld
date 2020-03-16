#include <planner_history.h>

using namespace cell_world;

const cell_world::Cell &Planner_history::last_record() {
    return _cells[_data_points[last_update]];
}

const cell_world::Cell &Planner_history::operator[](uint32_t iteration) {
    return _cells[_data_points[iteration]];
}

bool Planner_history::contains(uint32_t iteration) {
    if (iteration >= _data_points.size()) return false;
    return _data_points[iteration] != Not_found;
}

void Planner_history::clear() {
    _data_points.clear();
    record_count = 0;
}

void Planner_history::add_record(uint32_t iteration, const cell_world::Cell &cell) {
    int32_t index = _cells.find(cell);
    if (index == Not_found) {
        index = _cells.size();
        _cells.add(cell);
    }
    while (_data_points.size()<=iteration) _data_points.push_back(Not_found);
    _data_points[iteration] = index;
    last_update = iteration;
    record_count ++;
}
