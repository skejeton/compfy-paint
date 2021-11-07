#pragma once
#define PAIR_COUNT (1 << 16)

struct TilePosition
{
    int x, y;
};

struct Tilemap
{
    struct Pair
    {
        bool present;
        TilePosition pos;
        int value;
    };

    Pair *pairs;

    Tilemap();
    ~Tilemap();

    void
    put(int x, int y, int value);

    void
    erase(int x, int y);

    bool
    has_tile_at(int x, int y);

    bool
    iterating(int pos);
};
