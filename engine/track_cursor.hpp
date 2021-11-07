// Tracking cursor with wrapping and whatnot

#include <ext/raylib.h>
#include "viewport.hpp"

struct TrackCursor 
{
    int prev_x;
    int prev_y;
    // This will create a 1-frame latency
    // this is needed because raylib sometimes reports
    // incorrect mouse position info, the correct
    // mouse position info will be reported only the second frame
    bool restore_latch = false;
    Vector2 restore;
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
    
private:
    Vector2
    mouse_position();
};

