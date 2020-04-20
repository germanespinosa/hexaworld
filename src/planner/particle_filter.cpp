#include <planner/particle_filter.h>

using namespace cell_world;
using namespace std;

Particle_filter::Particle_filter(cell_world::Cell_group c, cell_world::Graph g, const Cell &goal, const Reward_config &rc ):
        Model(c),
        prey(goal),
        predator(g),
        hits(c.size(),0),
        _last_contact(Not_found){
    add_agent(prey);
    add_agent(predator);
}

Model &Particle_filter::get_valid_model () {
    if (status == Status::Running) end_episode();
    uint32_t particle_index = Chance::dice(particles.size());
    auto particle = particles[particle_index];
    prey.set_start_cell(_map[_current_prey_coordinates]);
    predator.set_fixed_start(_map[particle]);
    start_episode(_current_iteration);
    return *this;
}

void Particle_filter::create_particles(uint32_t k) {
    particles.clear();
    if (_last_contact == iteration) {
        particles.push_back(_predator_start_location);
    } else {
        prey.set_start_cell(_map[_prey_start_location]);
        if (_last_contact == Not_found) {
            predator.set_random_start();
        } else {
            predator.set_fixed_start(_map[_predator_start_location]);
        }
        for (auto &h:hits) h=0;
        for (uint32_t i = 0; i < k; i++) {
            if (_generate_particle()) {
                auto c = predator.cell().coordinates;
                hits[_map[c].id]++;
                particles.push_back(c);
            }
        }
    }
}

bool Particle_filter::_generate_particle() {
    if (status == cell_world::Model::Status::Running) end_episode();
    if (_last_contact == iteration) {
        start_episode(_current_iteration);
        return true;
    } else {
        uint32_t start_iteration = 0;
        if (_last_contact != Not_found) start_iteration = _last_contact;
        bool valid = true;
        start_episode(start_iteration);
        if (_last_contact != Not_found) prey.contact = false;
        for (uint32_t move_index = 0; move_index < trajectory.size() && !prey.contact; move_index++) {
            prey.set_move(trajectory[move_index]);
            if (!update()) {
                end_episode();
                valid = false;
                break;
            }
        }
        if (prey.contact) {
            valid = false;;
        }
        return valid;
    }
}

void Particle_filter::update_state(uint32_t i, cell_world::Coordinates prey_coordinates) {
    if (i == 0) {
        _prey_start_location = prey_coordinates;
        prey.set_start_cell(_map[_prey_start_location]);
    } else {
        auto move = prey_coordinates-_current_prey_coordinates;
        trajectory.push_back(move);
    }
    _current_prey_coordinates = prey_coordinates;
    _current_iteration = i;
}

void Particle_filter::update_state(uint32_t i, cell_world::Coordinates prey_coordinates, cell_world::Coordinates predator_coordinates) {
    _prey_start_location = prey_coordinates;
    _predator_start_location = predator_coordinates;
    _current_prey_coordinates = prey_coordinates;
    trajectory.clear();
    _current_iteration = i;
    _last_contact = i;
}
