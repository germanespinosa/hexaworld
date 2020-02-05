#pragma once

#include <string>

struct Cmd_parameter{
    bool present();
    int64_t int_value(int64_t);
    std::string value(std::string);
    double double_value(double);
private:
    std::string _content;
    bool _present = false;
    friend class Cmd_parameters;
};

struct Cmd_parameters{
    Cmd_parameters(int, char **);
    Cmd_parameter operator [](std::string);
private:
    int argc;
    char **args;
};

std::string get_parameter (std::string parameter_name, std::string , int , char *[]);
int64_t get_parameter_int (std::string parameter_name, int64_t , int , char *[]);
void print_hexaworld_help();
void print_hexamap_help();
void set_seed (int32_t seed);
bool find_parameter (std::string , int , char *[]);
void create_folder(std::string );
double round(double,int);
