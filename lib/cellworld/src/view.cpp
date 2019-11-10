#include<view.h>
#include<iostream>

using namespace ge211;
using namespace std;
using namespace cellworld;

View::View(World &world, Dimensions scene_dimensions):
    _world(world) , 
    _scene_dimensions(scene_dimensions)
{
    double sx,sy,lx,ly;
    lx = sx = _world[0].location.x;
    ly = sy = _world[0].location.y;
    for (unsigned int i=0;i<_world.size();i++){
        double x = _world[i].location.x;
        double y = _world[i].location.y;
        if (lx < x) lx = x;
        if (ly < y) ly = y;
        if (sx > x) sx = x;
        if (sy > y) sy = y;
    }
    double rx = (_scene_dimensions.width - 100) / (lx - sx);
    double ry = (_scene_dimensions.height - 100) / (ly - sy);
    _ratio = rx<ry?rx:ry;
    _cell_sprites.emplace_back((int)_ratio/2-1, Color{255,255,255,255});
    _cell_sprites.emplace_back((int)_ratio/2-1, Color{63, 63, 63, 255});
    _cell_sprites.emplace_back((int)_ratio/2-1, Color{255, 0, 0, 255});

}

Basic_position<int> View::_screen_location (const Basic_position<double> &location)
{
    return {
        (int)(_scene_dimensions.width / 2 + location.x * _ratio - _ratio  / 2), 
        (int)(_scene_dimensions.height / 2 + location.y * _ratio - _ratio  / 2)
    };
}

void View::draw(Sprite_set& sprites, vector<Agent_data> agents, string text)
{
    fps.reconfigure(Text_sprite::Builder(sans) << text);
    sprites.add_sprite(fps, {10, 10});
    
    for (unsigned int i =0 ; i< _world.size(); i++) {
        Cell &cell = _world[i];
        sprites.add_sprite(_cell_sprites[(int)cell.occluded], _screen_location(cell.location));
    }
    for (unsigned int i =0 ; i< agents.size(); i++) {
        sprites.add_sprite(_cell_sprites[2], _screen_location(_world[agents[i].coordinates].location));
    }
}