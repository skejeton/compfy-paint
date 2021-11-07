#pragma once
#include <engine/tileset.hpp>
#include <ui/ux.hpp>
#include <cstdint>

struct TilePicker
{
    struct Context
    {
        int id;
        TilePicker *picker;
    };
    Context contexts[1024];
    size_t context_count;
    int picked = 0;
    Ux *ux;
    Ui::Panel *root;

    TilePicker(Ux *ux);

    void
    use_tileset(Tileset &tileset);
};
