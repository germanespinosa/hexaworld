#pragma once

#include <string>
std::string get_parameter (std::string parameter_name, std::string default_value, int argc, char *args[]);
int64_t get_parameter_int (std::string parameter_name, int64_t default_value, int argc, char *args[]);
void print_hexaworld_help();
void print_hexamap_help();
void set_seed (int32_t seed);
bool find_parameter (std::string parameter_name, int argc, char *args[]);
