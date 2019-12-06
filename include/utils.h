//
// Created by german on 12/5/19.
//

#ifndef HEXAWORLD_UTILS_H
#define HEXAWORLD_UTILS_H

#include <string>
std::string get_parameter (const std::string parameter_name, const std::string default_value, int argc, char *args[]);
int64_t get_parameter_int (const std::string parameter_name, const int64_t default_value, int argc, char *args[]);
void print_help();

#endif //HEXAWORLD_UTILS_H
