#ifndef HEXAWORLD_PREY_H
#define HEXAWORLD_PREY_H
#include <cellworld.h>
#include "predator.h"
#include "hexaworld.h"
#define PREY_TYPE 1

struct Prey_state_action{
    uint32_t prey_cell_id;
    int32_t predator_cell_id;
    int action;
    uint32_t iteration;
    int last_seen;
    uint32_t  current_predator_cell_id;
};

struct Prey_expected_reward{
    uint32_t visits;
    double reward;
};

struct Prey_expected_reward_buffer{
    Prey_expected_reward_buffer(uint32_t size);
    ~Prey_expected_reward_buffer();
    Prey_expected_reward *values;
    bool load(std::string);
    bool save(std::string);
    uint32_t _size;
    Prey_expected_reward *operator [] (uint32_t);
};

struct Prey_config{
    uint32_t start;
    uint32_t goal;
    double success_reward;
    double failure_reward;
    double discount;
    double step_cost;
    cell_world::Color color;
    std::vector<int> action_probabilities;
};

struct Prey : cell_world::Agent {
    Prey(cell_world::World &, cell_world::Visibility &, Prey_config &);

    // called from the simulation
    void start_episode(const cell_world::State &) override;
    void update_state(const cell_world::State &) override;
    void end_episode(const cell_world::State &) override;
    uint32_t action_ready() override;
    uint32_t get_destination() override;

    // restart the prey
    void reset(uint32_t);

    // methods should be implemented
    virtual const std::vector<uint32_t> &get_options(uint32_t) = 0;
    virtual Prey_expected_reward *get_expected_rewards(Prey_state_action &) = 0;
    virtual void save_expected_reward (Prey_state_action &, double, uint32_t) = 0;
    ~Prey();
protected:
    Prey_config &_config;
    cell_world::World &_world;
    cell_world::Visibility &_visibility;
    std::vector<Prey_state_action> _history;
    uint32_t _predator_id;
    uint32_t _iteration;
    uint32_t _next_action;
    uint32_t _episodes;
    uint32_t _successes;
    double _reward_acum;
private:
    uint32_t _get_action(std::vector<uint32_t> &,Prey_expected_reward *,  int);
};

#endif //HEXAWORLD_PREY_H
