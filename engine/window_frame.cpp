#include "window_frame.hpp"
#include "ext/raylib.h"

static Viewport 
get_outer_viewport(WindowFrame *self)
{
    Viewport &v = self->viewport;
    return {
        v.at_x-2,
        v.at_y-22,
        v.sz_w+4,
        v.sz_h+24
    };
}

void
WindowFrame::update()
{
    Viewport outer = get_outer_viewport(this);
    // Check if in frame but not in viewport
    if (outer.mouse_inside() && !viewport.mouse_inside() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        prev_mouse_pos = GetMousePosition();
        if (outer.get_mouse_position().y > 20)
        {
            mode = RESIZING;
            SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
        }
        else
        {
            mode = DRAGGING;
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        mode = IDLING;
    }
    Vector2 current_mouse_pos = GetMousePosition();
    Vector2 motion = {
        current_mouse_pos.x-prev_mouse_pos.x,
        current_mouse_pos.y-prev_mouse_pos.y
    };
    if (mode == DRAGGING) 
    {
        viewport.at_x += motion.x;
        viewport.at_y += motion.y;
        prev_mouse_pos = current_mouse_pos;
    }
    else if (mode == RESIZING) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
        viewport.sz_w += motion.x;
        viewport.sz_h += motion.y;
        // Please no small windows
        if (viewport.sz_w < 20) viewport.sz_w = 20;
        if (viewport.sz_h < 20) viewport.sz_h = 20;
        prev_mouse_pos = current_mouse_pos;
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void
WindowFrame::draw()
{
    Viewport outer = get_outer_viewport(this);
    outer.draw_rectangle(0, 0, outer.sz_w, outer.sz_h, WHITE);
    outer.draw_text(title, 2, 0, 20, BLACK);
    viewport.draw_rectangle(0, 0, viewport.sz_w, viewport.sz_h, Color { 2, 10, 7, 255 });
}
