#pragma once
#include <stddef.h>
#include <ext/raylib.h>

struct Tileset
{
    struct Tile 
    {
        bool present;
        Color color;
    };

    Tile tiles[1024];
    Color
    get_color(size_t tile);
};


