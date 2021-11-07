#include "track_cursor.hpp"
#include <cstdio>
#include <math.h>

TrackCursor::TrackCursor() 
{
    prev_x = 0;
    prev_y = 0;
}

void
TrackCursor::start() 
{
    prev_x = GetMouseX();
    prev_y = GetMouseY();
    tracking = true;
}

Vector2
TrackCursor::motion()
{
    if (!tracking)
        return { 0, 0 };


    int curr_x = GetMouseX();
    int curr_y = GetMouseY();
    
    Vector2 diff = {
        float(prev_x-curr_x),
        float(prev_y-curr_y),
    };

    prev_x = curr_x;
    prev_y = curr_y;

    return diff;
}

Vector2
TrackCursor::mouse_position()
{
    if (restore_latch)
    {
        restore_latch = false;
        return restore;
    }
    return GetMousePosition();
}

Vector2
TrackCursor::wrap_motion(Viewport &viewport)
{
    if (!tracking)
        return { 0, 0 };

    Vector2 mpos = mouse_position();
    int curr_x = mpos.x;
    int curr_y = mpos.y;
    
    Vector2 diff = {
        float(prev_x-curr_x),
        float(prev_y-curr_y),
    };

    prev_x = curr_x;
    prev_y = curr_y;

    int p_x = prev_x;
    int p_y = prev_y;
    p_x -= viewport.rect.x();
    p_y -= viewport.rect.y();

    if (p_x <= 0 || p_y <= 0 || p_x >= viewport.rect.w() || p_y >= viewport.rect.h()) 
    {
        if (p_x <= 0) 
            p_x = viewport.rect.w()+p_x-1;
        
        if (p_y <= 0)
            p_y = viewport.rect.h()+p_y-1;

        if (p_x >= viewport.rect.w())
            p_x = p_x-viewport.rect.w()+1;

        if (p_y >= viewport.rect.h())
            p_y = p_y-viewport.rect.h()+1;

        p_x += viewport.rect.x();
        p_y += viewport.rect.y();

        SetMousePosition(p_x, p_y);
        restore = { p_x, p_y };
        restore_latch = true;
        prev_x = p_x;
        prev_y = p_y;
    }
    
    return diff;
}

void
TrackCursor::stop() 
{
    tracking = false;
}
