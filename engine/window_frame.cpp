#include "window_frame.hpp"
#include "ext/raylib.h"

static Viewport 
get_outer_viewport(WindowFrame *self)
{
    Viewport &v = self->viewport;
    return {{
        v.rect.x()-2,
        v.rect.y()-22,
        v.rect.w()+4,
        v.rect.h()+24
    }};
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
        viewport.rect.x() += motion.x;
        viewport.rect.y() += motion.y;
        prev_mouse_pos = current_mouse_pos;
    }
    else if (mode == RESIZING) {
        SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
        viewport.rect.w() += motion.x;
        viewport.rect.h() += motion.y;
        // Please no small windows
        if (viewport.rect.w() < 20) viewport.rect.w() = 20;
        if (viewport.rect.h() < 20) viewport.rect.h() = 20;
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
    outer.draw_rectangle(0, 0, outer.rect.w(), outer.rect.h(), { 50, 50, 50, 255 });
    outer.draw_text(title, 10, 6, 10, GRAY);
    viewport.draw_rectangle(0, 0, viewport.rect.w(), viewport.rect.h(), Color { 2, 10, 7, 255 });
}
