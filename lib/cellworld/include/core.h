#pragma once
#include <ge211.h>

namespace cellworld{
    struct Coordinates{
        int8_t x,y;
        
        bool operator ==(const Coordinates) const;
        bool operator !=(const Coordinates) const;
        Coordinates operator +=(const Coordinates);
        Coordinates operator +(const Coordinates) const;
        Coordinates operator -(const Coordinates) const;
        Coordinates operator -() const;
    };
} 