#pragma once
#include <ext/raylib.h>
#include <ui/ui.hpp>
#include "window_frame.hpp"
#include "track_cursor.hpp"
#include "tileset.hpp"
#include "tilemap.hpp"

struct TilemapEditor
{
    int tile_width;
    int tile_height;
    int tile_x_selected;
    int tile_y_selected;
    int panning_x;
    int panning_y;
    TrackCursor tracker;
    Tilemap tilemap;
    Ui::Panel *panel;
    // If previous tile was cleared, this will clear
    // All the consecutive ones touched when mouse is dragged
    bool clear_mode;
    int selected_tile = 0;

    void
    update();
    void
    draw(Tileset &tileset);
};
