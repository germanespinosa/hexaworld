//
// Created by german on 12/5/19.
//
#include <iostream>
#include <inttypes.h>

using namespace std;

void set_seed (int32_t seed){
    if (seed>0)
        srand(seed);
    else
        srand(time(NULL));
}

string get_parameter (const string parameter_name, const string default_value, int argc, char *args[]){
    for (int i=0; i < argc - 1; i++){
        if (parameter_name == args[i]) return args[i+1];
    }
    return default_value;
}

int64_t get_parameter_int (const std::string parameter_name, const int64_t default_value, int argc, char *args[]){
    string default_value_S = std::to_string(default_value);
    return strtoimax(get_parameter(parameter_name, default_value_S, argc, args).c_str(), NULL, 10);
}

void print_hexaworld_help(){
    cout << "Hexaworld simulation" << endl;
    cout << "--------------------" << endl;
    cout << "usage:" << endl;
    cout << "   hexaworld [filename.map] -seed [seed] -steps [steps] -episodes [episodes] -show [show] -width [width] -height [height]" << endl;
    cout << "parameters:" << endl;
    cout << "   -seed: random seed. Integer 0-65535 (default 0)" << endl;
    cout << "   -steps: number of steps per episode. Integer 1-1000 (default 100)" << endl;
    cout << "   -episodes: number of episodes. Integer 1-1000000 (default 1)" << endl;
    cout << "   -show: display the execution. Boolean true-false (default true)" << endl;
    cout << "   -width: windows width (only if show is true). Integer 640-1920 (default 1024)" << endl;
    cout << "   -height: windows height (only if show is true). Integer 480-1440 (default 768)" << endl;
    cout << endl;
}

void print_hexamap_help(){
    cout << "Hexaworld simulation map creation tool" << endl;
    cout << "--------------------------------------" << endl;
    cout << "usage:" << endl;
    cout << "   hexamap [filename.map] -seed [seed] -size [size] -occlusions [occlusions]" << endl;
    cout << "parameters:" << endl;
    cout << "   -seed: random seed. Integer 0-65535 (default 0)" << endl;
    cout << "   -size: number of cells in a side. Odd integer 5-19 (default 11)" << endl;
    cout << "   -occlusions: number of occlusions. Integer 0-200 (default 80)" << endl;
    cout << endl;
}