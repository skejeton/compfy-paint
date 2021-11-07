#include "viewport.hpp"
#include "ext/raylib.h"
#include <cstdio>

Vector2
Viewport::get_mouse_position()
{
    Vector2 base = GetMousePosition();
    base.x -= rect.x();
    base.y -= rect.y();
    return base;
}

bool
Viewport::mouse_inside()
{
    Vector2 pos = get_mouse_position();
    return pos.x >= 0 && pos.y >= 0 && pos.x <= rect.w() && pos.y <= rect.h();
}

void
Viewport::draw_line(int x1, int y1, int x2, int y2, Color color)
{
    DrawLine(x1+rect.x(), y1+rect.y(), x2+rect.x(), y2+rect.y(), color);
}

void
Viewport::draw_text(const char *txt, int x, int y, int size, Color color)
{
    DrawText(txt, x+rect.x(), y+rect.y(), size, color);
}

void
Viewport::draw_rectangle(int x, int y, int w, int h, Color color)
{
    DrawRectangle(x+rect.x(), y+rect.y(), w, h, color);
}

void
Viewport::draw_pixel(int x, int y, Color color)
{
    DrawPixel(x+rect.x(), y+rect.y(), color);
}
