#pragma once
#include <ext/raylib.h>
#include <ui/ui.hpp>
#include "window_frame.hpp"
#include "track_cursor.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"

struct TilemapEditor
{
    vi2 last_tile_selected;
    vi2 tile_selected;
    bool placing = false;
    TrackCursor tracker;
    Tilemap tilemap;
    Ui::Panel *panel;
    float zoom_target = 8.0;
    Camera2D camera = {
        .offset = { 0, 0 },
        .target = { 0, 0 },
        .zoom = 8.0,
    };
    int selected_tile = 0;


    void
    update();

    void
    draw(Tileset &tileset);
};
