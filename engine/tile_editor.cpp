#include "tile_editor.hpp"
#include "ext/raylib.h"
#include <stdio.h>
#include <math.h>


// This will plot line from last tile x/y to tile x/y
static void
plot_line(TilemapEditor *self, bool erase = false)
{
    int x0 = self->last_tile_selected.x;
    int y0 = self->last_tile_selected.y;
    int x1 = self->tile_selected.x;
    int y1 = self->tile_selected.y;

    int dx = abs(x1-x0);
    int sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0);
    int sy = y0<y1 ? 1 : -1;
    int err = dx+dy;  /* error value e_xy */
    while (true)
    {
        if (erase)
            self->tilemap.erase(x0, y0);
        else
            self->tilemap.put(x0, y0, self->selected_tile);
        if (x0 == x1 && y0 == y1) 
            break;
        int e2 = 2*err;
        if (e2 >= dy) 
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) 
        {
            err += dx;
            y0 += sy;
        }
    }
    self->last_tile_selected = self->tile_selected;
}

void
TilemapEditor::update()
{
    Viewport viewport = panel->viewport();
    if (viewport.mouse_inside() && IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        tracker.start();
    if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON))
        tracker.stop();
            
    Vector2 motion = tracker.wrap_motion(viewport);
    camera.target.x += motion.x/camera.zoom;
    camera.target.y += motion.y/camera.zoom;
    Vector2 mouse_pos = viewport.get_mouse_position();

    // if (viewport.mouse_inside() && GetMouseWheelMove() != 0)
    {
    
        double motion = viewport.mouse_inside() ? GetMouseWheelMove() : 0;
        Vector2 screen_old = GetScreenToWorld2D(
            {mouse_pos.x, mouse_pos.y},
            camera
        );
        if (motion != 0)
            zoom_target *= pow(2, motion);

        if (zoom_target < 1.0) zoom_target = 1.0;
        if (zoom_target > 64.0) zoom_target = 64.0;
        camera.zoom += (zoom_target-camera.zoom)/4.0;
        
        Vector2 screen_new = GetScreenToWorld2D(
            {mouse_pos.x, mouse_pos.y},
            camera
        );

        camera.target.x += (screen_old.x - screen_new.x);
        camera.target.y += (screen_old.y - screen_new.y);
    }
    
    Vector2 screen = GetScreenToWorld2D(
        {mouse_pos.x, mouse_pos.y},
        camera
    );
    tile_selected.x = floor(screen.x);
    tile_selected.y = floor(screen.y);

    if (viewport.mouse_inside() && 
        (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || 
         IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)))
    {
        placing = true;
        last_tile_selected = tile_selected;
    }
    // FIXME: This will stop placing if you start 
    // with left button but release the right one
    if (placing && 
        (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)))
    {
        placing = false;
    }
    if (viewport.mouse_inside() && placing && IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT))
    {
        plot_line(this, true);
    }
    if (viewport.mouse_inside() && placing && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT))
    {
        plot_line(this);
    }
}

void
TilemapEditor::draw(Tileset &tileset)
{
    Viewport viewport = panel->viewport();
    int width = viewport.rect.w(),
        height = viewport.rect.h();

    // Since the amount of cells will be truncated
    // I'll add 5 to make it guarantee to draw all
    int col_count = width/camera.zoom+5;
    int row_count = height/camera.zoom+5;

    Vector2 target = camera.target;
    Vector2 ofs = camera.target;
    ofs.x -= floor(camera.target.x); ofs.y -= floor(camera.target.y);

    // camera.offset = ;
    camera.target = ofs;//{ 0, 0 };
    BeginMode2D(camera);
    // Draw grid cells
    if (camera.zoom > 4.0)
    {
        for (int i = 0; i < row_count; i += 1)
        {
            Color color = DARKGRAY;
            if ((i+int(floor(target.y))) % 16 == 0)
                color = WHITE;
            
            int y = i;
            viewport.draw_line(
                0, y,
                width/camera.zoom*2, y,
                ColorAlpha(color, 0.1)
            );
        }

        for (int i = 0; i < col_count; i += 1)
        {
            Color color = DARKGRAY;
            if ((i+int(floor(target.x))) % 16 == 0)
                color = WHITE;
                
            int x = i;
            viewport.draw_line(
                x, 0,
                x, height/camera.zoom*2,
                ColorAlpha(color, 0.1)
            );
        }
    }

    EndMode2D();
    camera.offset = { 0, 0 };
    camera.target = target;
    BeginMode2D(camera);

    int tile_count = 0;

    for (int i = 0; tilemap.iterating(i); i += 1)
    {
        Tilemap::Pair &pair = tilemap.pairs[i];
        if (!pair.present)
            continue;
        tile_count += 1; 
        if (camera.zoom <= 1.0)
        {
            viewport.draw_pixel(
                pair.pos.x,
                pair.pos.y,
                tileset.get_color(pair.value)
            );
        }
        else
        {
            viewport.draw_rectangle(
                pair.pos.x,
                pair.pos.y,
                1, 1,
                tileset.get_color(pair.value)
            );
        }
    }
    

    // Draw the selected tile
    if (viewport.mouse_inside())
        viewport.draw_rectangle(
            tile_selected.x,
            tile_selected.y,
            1, 1,
            ColorAlpha(tileset.get_color(selected_tile), 0.1)
        );

    EndMode2D();
    camera.target = target;

    // Draw the selected selected tile indicator
    viewport.draw_text(
        TextFormat("%d, %d", tile_selected.x, tile_selected.y),
        10, height-20, 10, ColorAlpha(WHITE, 0.5)
    );
}
