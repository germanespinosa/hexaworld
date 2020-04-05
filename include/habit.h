#pragma once
#include <hexaworld.h>
#include <cell_world.h>
#include <option.h>
#include <reward_config.h>

struct Habit_action{
    Habit_action (cell_world::Move, uint32_t ,double);
    cell_world::Move move;
    uint32_t visits = 0;
    double reward = 0;
};

struct Habit_value{
    std::vector<Habit_action> actions;
    double length = 0;
    uint32_t fails = 0;
    uint32_t successes = 0;
    uint32_t unknowns = 0;
    uint32_t visits = 0;
    cell_world::Move policy();
};

struct Habit : Option{
    explicit Habit (Option);
    std::vector<Habit_value> values;
    bool load(const std::string&);
    bool save(const std::string&);
    void add_reward(uint32_t, uint32_t,Reward_config &, Episode_result, uint32_t);
    void end_episode(uint32_t, Episode_result);
    cell_world::Move policy(const cell_world::Cell &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Cell_group &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Cell_group &, const std::string &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Graph &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Graph &, const std::string &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Graph &, cell_world::Graph &);
    static std::vector<Habit> get_habits(cell_world::Graph &, cell_world::Graph &, cell_world::Graph &, const std::string &);
private:
    std::string _file_name();
    const std::string _extension = ".habit";
};

struct Habit_set {
    Habit_set(const cell_world::World &, const cell_world::Cell_group &);
    std::vector<std::reference_wrapper<Habit>> operator [](const cell_world::Cell &);
    std::vector<Habit> habits;
    cell_world::Cell_group destinations;
    cell_world::Graph options;
};
