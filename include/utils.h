#pragma once
#include <string>
#include <ctime>

struct Cmd_parameter{
    bool present();
    int64_t int_value(int64_t);
    int64_t int_value();
    std::string value(std::string);
    std::string value();
    double double_value(double);
private:
    std::string _content;
    bool _present = false;
    friend class Cmd_parameters;
};

struct Cmd_parameters{
    Cmd_parameters(int, char **);
    Cmd_parameter operator [](std::string);
    Cmd_parameter operator [](uint32_t);
private:
    int argc;
    char **args;
};

struct Stop_watch{
    Stop_watch();
    double elapsed();
    double tick();
    static std::string to_string(double);
private:
    clock_t _clock;
};

void print_hexaworld_help();
void print_hexamap_help();
void set_seed (int32_t seed);
void create_folder(std::string );
double round(double,int);
