#pragma once
#include <ge211.h>
#include <core.h>
#include <world.h>
#include <agent.h>

namespace cellworld{
     struct View
    {
        View(World &world, std::vector<Agent> &, const ge211::Dimensions);
        void draw (ge211::Sprite_set& sprites, std::string);
        private:
        ge211::Basic_position<int> _screen_location (const ge211::Basic_position<double> & );
        World &_world;
        std::vector<Agent> _agents;
        std::vector<ge211::Circle_sprite> _cell_sprites;
        std::vector<ge211::Circle_sprite> _agent_sprites;
        ge211::Font sans{"sans.ttf", 30};
        ge211::Text_sprite fps;
        const ge211::Dimensions _scene_dimensions ;
        double _get_ratio();
        double _ratio=0;
    };
}