#ifndef HEXAWORLD_PREY_H
#define HEXAWORLD_PREY_H
#include <cellworld.h>
#include "predator.h"
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
    cellworld::Color color;
    std::vector<int> action_probabilities;
};

struct Prey : cellworld::Agent {
    Prey(cellworld::World &, cellworld::Visibility &, Prey_config &);

    // called from the simulation
    void start_episode(const cellworld::State &) override;
    void update_state(const cellworld::State &) override;
    void end_episode(const cellworld::State &) override;
    uint32_t action_ready() override;
    uint32_t get_destination() override;

    // restart the prey
    void reset(uint32_t);

    // methods should be implemented
    virtual std::vector<uint32_t> get_options(uint32_t) = 0;
    virtual Prey_expected_reward *get_expected_rewards(Prey_state_action &) = 0;
    virtual void save_expected_reward (Prey_state_action &, double, uint32_t) = 0;
    ~Prey();
protected:
    Prey_config &_config;
    cellworld::World &_world;
    cellworld::Visibility &_visibility;
    std::vector<Prey_state_action> _history;
    uint32_t _predator_id;
    uint32_t _iteration;
    uint32_t _next_action;
    uint32_t _episodes;
    uint32_t _successes;
private:
    uint32_t _get_action(std::vector<uint32_t> &,Prey_expected_reward *,  int);
};

#endif //HEXAWORLD_PREY_H
