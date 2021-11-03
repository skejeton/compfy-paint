#include "tileset.hpp"

Color
Tileset::get_color(size_t tile)
{   
    if (tile == 0)
        return Color { 0, 0, 0, 0 };
    tile -= 1;
    if (tile > 1024)
    {
        return Color { 0, 0, 0, 0 };
    }
    return tiles[tile].color;
}
