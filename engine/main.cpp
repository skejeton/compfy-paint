#include <cstdio>
extern "C" {
#include <ext/raylib.h>
}
#include <engine/tile_editor.hpp>
#include <engine/tileset.hpp>
#include <core/dispatch.hpp>
#include <engine/tile_picker.hpp>
#include <ui/ux.hpp>
#include "editor.hpp"
#include <memory.h>


int
main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1920/2+10, 1080/2, "compfy paint");
    Editor editor {};

    // Disable exit on ESC
    SetExitKey(0);
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        editor.tick();
    }
    return 0;
}

