#pragma once
#include <string>
#include <ctime>
#include <cell_world.h>


struct Cmd_parameter{
    bool present();
    bool file_exist();
    bool file_exist(const std::string&);
    bool range(double, double);
    int64_t int_value(int64_t);
    int64_t int_value();
    std::string value(std::string);
    std::string value();
    double double_value(double);
    double double_value();
    Cmd_parameter & default_value(std::string);
    Cmd_parameter & default_value(int);
    Cmd_parameter & check_present(const std::string&);
    Cmd_parameter & check_present();
    Cmd_parameter & check_file_exist(const std::string&, const std::string&);
    Cmd_parameter & check_file_exist(const std::string&);
    Cmd_parameter & check_range(int, int, const std::string&);
    Cmd_parameter & check_range(int, int);
    operator std::string();
    operator double ();
private:
    std::string _name;
    std::string _content;
    bool _present = false;
    friend class Cmd_parameters;
};

struct Cmd_parameters{
    Cmd_parameters(int, char **);
    Cmd_parameter operator [](const std::string&);
    Cmd_parameter operator [](uint32_t);
private:
    int argc;
    char **args;
};

struct Stop_watch{
    Stop_watch();
    bool time_out (double);
    bool time_out (uint32_t);
    void stop();
    double elapsed();
    double tick();
    void reset();
    static std::string to_string(double);
    clock_t _clock;
    bool _running;
};

void print_hexaworld_help();
void print_hexamap_help();
void set_seed (int32_t seed);
void create_folder(std::string );
double round(double,int);
void print_history(std::vector<cell_world::Coordinates> history);