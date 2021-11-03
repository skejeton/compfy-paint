#include "tilemap.hpp"
#include <stdio.h>
#include <stdlib.h>

#define PAIR_COUNT 1024

Tilemap::Tilemap() 
{
    this->pairs = (Pair*)calloc(1, sizeof(Pair[PAIR_COUNT]));
}

Tilemap::~Tilemap() 
{
    free(this->pairs);
}

void
Tilemap::put(int x, int y, int value)
{
    // We really don't want to overdraw tiles
    if (has_tile_at(x, y))
        erase(x, y);

    for (int i = 0; i < PAIR_COUNT; i += 1)
    {
        if (!pairs[i].present) 
        {
            pairs[i].pos = { x, y };
            pairs[i].present = true;
            pairs[i].value = value;
            return;
        }
    }
}

void
Tilemap::erase(int x, int y)
{
    for (int i = 0; i < PAIR_COUNT; i += 1)
    {
        if (pairs[i].present && 
            pairs[i].pos.x == x &&
            pairs[i].pos.y == y) 
        {
            pairs[i].present = false;
            return;
        }
    }
}

bool
Tilemap::has_tile_at(int x, int y)
{
    for (int i = 0; i < PAIR_COUNT; i += 1)
    {
        if (pairs[i].pos.x == x &&
            pairs[i].pos.y == y) 
        {
            return pairs[i].present;
        }
    }
    return false;
}

bool
Tilemap::iterating(int pos)
{
    return pos < PAIR_COUNT;
}
