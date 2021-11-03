#include "tile_editor.hpp"
#include "ext/raylib.h"
#include <stdio.h>
#include <math.h>

#define selfbind(x) [](void *self___){auto self = (decltype(this))(self___); x}

void
TilemapEditor::update()
{
    Viewport viewport = panel->viewport();
    if (viewport.mouse_inside() && IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        tracker.start();
    if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON))
        tracker.stop();
    Vector2 motion = tracker.wrap_motion(viewport);
    panning_x += motion.x;
    panning_y += motion.y;
    Vector2 mouse_pos = viewport.get_mouse_position();
    tile_x_selected = (int)floor((float)(mouse_pos.x + panning_x) / (float)tile_width);
    tile_y_selected = (int)floor((float)(mouse_pos.y + panning_y) / (float)tile_height);

    if (viewport.mouse_inside() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        clear_mode = tilemap.has_tile_at(tile_x_selected, tile_y_selected);
    }
    if (viewport.mouse_inside() && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
    {
        if (clear_mode)
            tilemap.erase(tile_x_selected, tile_y_selected);
        else
            tilemap.put(tile_x_selected, tile_y_selected, selected_tile);
    }
}

void 
TilemapEditor::draw(Tileset &tileset) 
{
    Viewport viewport = panel->viewport();
    int width = viewport.sz_w,
        height = viewport.sz_h;

    // Since the amount of cells will be truncated 
    // I'll add 1 to make it guarantee to draw all
    int row_count = height / tile_height + 2;
    int col_count = width  / tile_width + 2;

    // Draw grid cells
    for (int i = 0; i < row_count; i += 1)
    {
        int y = i * tile_height - panning_y % tile_height;

        viewport.draw_line(0, y, width, y, ColorAlpha(WHITE, 0.1));
    }

    for (int i = 0; i < col_count; i += 1)
    {
        int x = i * tile_width - panning_x % tile_width;

        viewport.draw_line(x, 0, x, height, ColorAlpha(WHITE, 0.1));
    }

    for (int i = 0; tilemap.iterating(i); i += 1)
    {
        Tilemap::Pair &pair = tilemap.pairs[i];
        if (!pair.present) 
            continue;
        viewport.draw_rectangle(
            (pair.pos.x - panning_x/tile_width)*tile_width - panning_x % tile_width,
            (pair.pos.y - panning_y/tile_height)*tile_height - panning_y % tile_height,
            tile_width, tile_height,
            tileset.get_color(pair.value)
        );
    }
    
    // Draw the selected tile
    if (viewport.mouse_inside())
        viewport.draw_rectangle(
            (tile_x_selected - panning_x/tile_width)*tile_width - panning_x % tile_width,
            (tile_y_selected - panning_y/tile_height)*tile_height - panning_y % tile_height,
            tile_width, tile_height,
            ColorAlpha(tileset.get_color(selected_tile), 0.1)
        );

    // Draw the selected selected tile indicator
    viewport.draw_text(
        TextFormat("%d, %d", tile_x_selected, tile_y_selected),
        10, height-20, 10, ColorAlpha(WHITE, 0.5)
    );
}
