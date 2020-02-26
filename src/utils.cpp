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
#include <utility>


void create_folder(string path){
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

Cmd_parameters::Cmd_parameters(int c, char **s) {
    argc = c;
    args = s;
}

Cmd_parameter Cmd_parameters::operator[](const std::string& name) {
    Cmd_parameter cp;
    cp._name = name;
    for (int i=0; i < argc; i++){
        if (name == args[i]) {
            if (i+1<argc && args[i+1][0]!='-') cp._content = args[i+1];
            cp._present = true;
            return cp;
        }
    }
    return cp;
}

Cmd_parameter Cmd_parameters::operator[](uint32_t index) {
    Cmd_parameter cp;
    cp._name = "position " + std::to_string(index);
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

bool Cmd_parameter::file_exist(const std::string& extension) {
    struct stat buffer{};
    return stat((_content+extension).c_str(), &buffer) == 0;
}

bool Cmd_parameter::file_exist() {
    return file_exist("");
}

bool Cmd_parameter::range(double min, double max) {
    double  v = double_value();
    return v>=min && v<=max;
}

double Cmd_parameter::double_value() {
    return double_value(0);
}

Cmd_parameter &Cmd_parameter::default_value(std::string value) {
    if (!present())_content = value;
    return *this;
}

Cmd_parameter &Cmd_parameter::default_value(int value) {
    return default_value(std::to_string(value));
}

Cmd_parameter &Cmd_parameter::check_present() {
    return check_present("Missing parameter: " + _name);
}

Cmd_parameter &Cmd_parameter::check_present(const std::string& message) {
    if (!present()) {
        cerr << message << endl;
        exit(0);
    }
    return *this;
}

Cmd_parameter &Cmd_parameter::check_file_exist(const std::string& extension ,const std::string& message) {
    if (!file_exist(extension)) {
        cerr << message << endl;
        exit(0);
    }
    return *this;
}

Cmd_parameter &Cmd_parameter::check_file_exist(const std::string& extension) {
    return check_file_exist(extension,"File " + _content + extension + " not found.");
}

Cmd_parameter &Cmd_parameter::check_range(int min, int max, const std::string& message) {
    if (!range(min, max)){
        cerr << message << endl;
        exit(0);
    }
    return *this;
}

Cmd_parameter &Cmd_parameter::check_range(int min, int max) {
    return check_range(min, max, "Parameter " + _name + " value " + _content + " not within " + std::to_string(min) + "-" + std::to_string(max) + " range.");
}

Cmd_parameter::operator std::string() {
    return _content;
}

Cmd_parameter::operator double() {
    return double_value();
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
    double e = elapsed();
    reset();
    return e;
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

bool Stop_watch::time_out(double period) {
    return elapsed()>=period;
}

void Stop_watch::reset() {
    _clock = clock();
}
