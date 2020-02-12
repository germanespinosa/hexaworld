//
// Created by german on 12/5/19.
//
#include <iostream>
#include <cinttypes>
#include <cmath>
#include "utils.h"

using namespace std;

void set_seed (int32_t seed){
    if (seed>0)
        srand(seed);
    else
        srand(time(NULL));
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

#include <sys/stat.h>
#include <unistd.h>
#include <utils.h>
#include <sstream>


void create_folder(string path){
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

Cmd_parameters::Cmd_parameters(int c, char **s) {
    argc = c;
    args = s;
}

Cmd_parameter Cmd_parameters::operator[](std::string name) {
    for (int i=0; i < argc - 1; i++){
        if (name == args[i]) {
            Cmd_parameter cp;
            if (i+1<argc && args[i+1][0]!='-') cp._content = args[i+1];
            cp._present = true;
            return cp;
        }
    }
    return Cmd_parameter();
}

Cmd_parameter Cmd_parameters::operator[](uint32_t index) {
    Cmd_parameter cp;
    if (index>=argc) return cp;
    cp._present = true;
    cp._content = args[index];
    return cp;
}

bool Cmd_parameter::present() {
    return _present;
}

int64_t Cmd_parameter::int_value(int64_t d) {
    if (_present) return strtoimax(_content.c_str(), NULL, 10);
    return d;
}

std::string Cmd_parameter::value(std::string d) {
    if (_present) return _content;
    return d;
}

double Cmd_parameter::double_value(double d) {
    if (_present) return strtod(_content.c_str(), NULL);
    return d;
}

std::string Cmd_parameter::value() {
    return value("");
}

int64_t Cmd_parameter::int_value() {
    return int_value(0);
}

double round(double v, int d){
    double m = pow(10,d);
    v=v*m;
    v=(int)v;
    return v/m;
}

Stop_watch::Stop_watch() {
    _clock = clock();
}

double Stop_watch::tick() {
    clock_t stop_clock = clock();
    double time_spent = (double)(stop_clock - _clock) / CLOCKS_PER_SEC;
    _clock = stop_clock;
    return time_spent;
}

double Stop_watch::elapsed() {
    clock_t stop_clock = clock();
    double time_spent = (double)(stop_clock - _clock) / CLOCKS_PER_SEC;
    return time_spent;
}

std::string Stop_watch::to_string(double t) {
    int seg = t;
    int s = seg % 60;
    int m = (seg / 60) % 60;
    int h = ( seg / 3600);
    std::stringstream fmt;
    if (h > 0) {
        fmt << h << "h ";
    }
    if (m > 0) {
        fmt << m << "m ";
    }
    fmt << s << "s ";
    return fmt.str();
}
