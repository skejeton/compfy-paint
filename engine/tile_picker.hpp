#pragma once
#include <engine/tileset.hpp>

struct TilePicker 
{
    struct Entry 
    {
        bool present;
        int value;
    };
    int picked;

    Entry *entries;  

    TilePicker();
    ~TilePicker();
    
    void
    add_tile(int value);

    bool
    update();

    void
    draw(Tileset &tileset);
};
