#include "viewport.hpp"
#include "ext/raylib.h"
#include <cstdio>

Vector2
Viewport::get_mouse_position()
{
    Vector2 base = GetMousePosition();
    base.x -= at_x;
    base.y -= at_y;
    return base;
}

bool
Viewport::mouse_inside()
{
    Vector2 pos = get_mouse_position();
    return pos.x >= 0 && pos.y >= 0 && pos.x <= sz_w && pos.y <= sz_h;
}

void
Viewport::draw_line(int x1, int y1, int x2, int y2, Color color)
{
    BeginScissorMode(at_x, at_y, sz_w, sz_h);
        DrawLine(x1+at_x, y1+at_y, x2+at_x, y2+at_y, color);
    EndScissorMode();
}

void
Viewport::draw_text(const char *txt, int x, int y, int size, Color color)
{
    BeginScissorMode(at_x, at_y, sz_w, sz_h);
        DrawText(txt, x+at_x, y+at_y, size, color);
    EndScissorMode();
}

void
Viewport::draw_rectangle(int x, int y, int w, int h, Color color)
{
    BeginScissorMode(at_x, at_y, sz_w, sz_h);
        DrawRectangle(x+at_x, y+at_y, w, h, color);
    EndScissorMode();
}
