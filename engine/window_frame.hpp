// Simple window frame
#pragma once

#include "viewport.hpp"

struct WindowFrame
{
    enum Mode {
        IDLING,
        DRAGGING,
        RESIZING
    };
    Viewport viewport;
    const char *title;
    Mode mode = IDLING;
    Vector2 prev_mouse_pos = { 0, 0 };

    void
    update();

    void
    draw();
};
