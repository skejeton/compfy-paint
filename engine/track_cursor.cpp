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
TrackCursor::wrap_motion(Viewport &viewport)
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


    // Do this atrocity because i don't know
    // raylib probably reports wrong mouse position
    // if you remove this it will stutter
    if (diff.x > viewport.sz_w)
        diff.x = 0;
    if (diff.y > viewport.sz_h)
        diff.y = 0;

    int p_x = prev_x;
    int p_y = prev_y;
    p_x -= viewport.at_x;
    p_y -= viewport.at_y;

    if (p_x < 0 || p_y < 0 || p_x > viewport.sz_w || p_y > viewport.sz_h) 
    {
        if (p_x < 0) 
            p_x = viewport.sz_w+p_x;
        
        if (p_y < 0)
            p_y = viewport.sz_h+p_y;

        if (p_x > viewport.sz_w)
            p_x = p_x-viewport.sz_w;

        if (p_y > viewport.sz_h)
            p_y = p_y-viewport.sz_h;

        p_x += viewport.at_x;
        p_y += viewport.at_y;

        SetMousePosition(p_x, p_y);
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
