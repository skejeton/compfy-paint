/* Handles drawing and input into a viewport */
#pragma once
#include <ext/raylib.h>
#include <core/math.hpp>

struct Viewport
{
    recti rect;

    Vector2
    get_mouse_position();

    bool
    mouse_inside();

    void
    draw_line(int x1, int y1, int x2, int y2, Color color); 

    void
    draw_text(const char *txt, int x, int y, int size, Color color);

    void
    draw_rectangle(int x, int y, int w, int h, Color color);
    
    void
    draw_pixel(int x, int y, Color color);
};
