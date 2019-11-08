#include <world.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
using namespace cellworld;
using namespace ge211;

Cell::Cell (uint32_t index, Coordinates coordinates, Basic_position<double> location, std::vector<uint32_t> connections, bool occluded)
{
    this->index = index;
    this->location = location;
    this->coordinates = coordinates;
    this->connections = connections;
    this->occluded=occluded;
}

Cell::Cell(){
    this->index = 0;
    this->location = {0,0};
    this->coordinates = {0,0};
    this->connections = {};
    this->occluded= false;
   
}


bool World::add(const Cell cell){
    if (cells.size() != cell.index) return false;
    cells.push_back(cell);
    return true;
}
bool World::load(const std::string filepath){
    cells.clear();
    std::ifstream ifile;
    ifile.open(filepath.c_str());
    string line;
    while (getline(ifile, line)){
        istringstream ss(line);
        int16_t cx,cy;
        Cell cell;
        ss >> cell.index;
        ss >> cx;
        ss >> cy;
        ss >> cell.location.x;
        ss >> cell.location.y;
        ss >> cell.occluded;
        cell.coordinates.x = cx;
        cell.coordinates.y = cy;
        while (!ss.eof()){
            int32_t ci = -1;
            ss >> ci;
            if (ci>=0)
                cell.connections.push_back(ci);
        }
        if (!add(cell)) return false;
    }
    return true;
}
bool World::save(const std::string filepath) const{
    std::ofstream  ofile;
    ofile.open(filepath.c_str());
    for (unsigned int i=0;i<cells.size();i++){
        ofile << i << " " 
        << (int16_t)cells[i].coordinates.x << " "
        << (int16_t)cells[i].coordinates.y << " "
        << cells[i].location.x << " "
        << cells[i].location.y << " "
        << cells[i].occluded ;
       for (unsigned int c=0;c<cells[i].connections.size();c++){
           ofile << " " << cells[i].connections[c];
       }
       ofile << std::endl;
    }
    ofile << "-1";
    return true;
}
uint32_t World::size() const{
    return cells.size();
}

int32_t World::find (const Coordinates& coordinates) const{
    for (unsigned int i=0; i<cells.size(); i++)
        if (cells[i].coordinates==coordinates) return i;
    return -1;
}

Cell &World::operator[](const uint32_t& index){
    return cells[index];
}

Cell &World::operator[](const Coordinates& coordinates){
    return cells[find(coordinates)];
}

double World::distance(const Cell &c0, const Cell &c1) const {
    return sqrt(pow(c1.location.y-c0.location.y,2) + pow(c1.location.x-c0.location.x,2));
}
double World::distance(const Cell  &c0, const Cell &c1, const Cell &c2) const {
    return abs((c2.location.y-c1.location.y) * c0.location.x-(c2.location.x-c1.location.x) *c0.location.y+c2.location.x*c1.location.y-c2.location.y * c1.location.x) / sqrt(pow(c2.location.y-c1.location.y,2)+pow(c2.location.x-c1.location.x,2));
}

