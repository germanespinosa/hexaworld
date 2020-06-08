#include <agents/preys/planner_prey.h>
#include <agents/predator.h>
#include <cell_world.h>

struct Particle_filter: cell_world::Model{
    Particle_filter(cell_world::Cell_group, cell_world::Graph, const cell_world::Cell &, const Reward_config &, cell_world::Paths &, unsigned int);
    void update_state(unsigned int ,cell_world::Coordinates);
    void update_state(unsigned int ,cell_world::Coordinates, cell_world::Coordinates );
    cell_world::Model &get_valid_model();
    cell_world::Paths &paths;
    Predator predator;
    Planner_prey prey;
    std::vector<cell_world::Move> trajectory;
    std::vector<cell_world::Coordinates> particles;
    void create_particles (unsigned int );
    std::vector<unsigned int> hits;
    void start();
private:
    cell_world::Coordinates _current_prey_coordinates;
    cell_world::Coordinates _prey_start_location;
    cell_world::Coordinates _predator_start_location;
    unsigned int _current_iteration;
    int _last_contact;
    bool _generate_particle_no_observations();
    bool _generate_particle_observations();
};