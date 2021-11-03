// Tracking cursor with wrapping and whatnot

#include <ext/raylib.h>
#include "viewport.hpp"

struct TrackCursor 
{
    int prev_x;
    int prev_y;
    bool tracking = false;

    TrackCursor();

    void
    start();

    Vector2
    motion();

    Vector2
    wrap_motion(Viewport &viewport);

    void
    stop();
};

