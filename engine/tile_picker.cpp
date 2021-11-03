#include "tile_picker.hpp"
#include "ext/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define ENTRY_COUNT 1024

TilePicker::TilePicker()
{
    entries = (Entry*)calloc(1, sizeof(Entry[ENTRY_COUNT]));
}

TilePicker::~TilePicker()
{
    free(entries);
}

void
TilePicker::add_tile(int value)
{
    for (int i = 0; i < ENTRY_COUNT; i += 1)
    {
        if (!entries[i].present)
        {
            entries[i].present = value;
            entries[i].value = value;
            return;
        }
    }
}

constexpr static int
present_entries(TilePicker *self) 
{
    int count = 0;
    for (int i = 0; i < ENTRY_COUNT; i += 1)
    {
        count += self->entries[i].present;
    }
    return count;
}

bool
TilePicker::update()
{
    const int RECT_SIZE = 64;
    const int PADDING = 10;
    const int OVERALL = RECT_SIZE+PADDING*2;
    const int FRAME_HEIGHT = OVERALL;
    const int FRAME_WIDTH  = OVERALL*present_entries(this);
    const int FRAME_X = GetScreenWidth()-FRAME_WIDTH-10;
    const int FRAME_Y = GetScreenHeight()-OVERALL-10;
    int x = FRAME_X;
    int y = FRAME_Y;
    DrawRectangle(FRAME_X, FRAME_Y, FRAME_WIDTH, FRAME_HEIGHT, BLACK);
    for (int i = 0; i < ENTRY_COUNT; i += 1)
    {
        if (!entries[i].present)
            continue;
        bool mouse_in = CheckCollisionPointRec(
            GetMousePosition(),
            {float(x+PADDING), float(y+PADDING), RECT_SIZE, RECT_SIZE}
        );
        if (mouse_in && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            picked = i;
            printf("Picking %i\n", i);
        }
        x += OVERALL;
    }
    return CheckCollisionPointRec(GetMousePosition(), {(float)FRAME_X, (float)FRAME_Y, (float)FRAME_WIDTH, FRAME_HEIGHT});
}

void
TilePicker::draw(Tileset &tileset)
{
    const int RECT_SIZE = 64;
    const int PADDING = 10;
    const int OVERALL = RECT_SIZE+PADDING*2;
    const int FRAME_HEIGHT = OVERALL;
    const int FRAME_WIDTH  = OVERALL*present_entries(this);
    const int FRAME_X = GetScreenWidth()-FRAME_WIDTH-10;
    const int FRAME_Y = GetScreenHeight()-OVERALL-10;
    int x = FRAME_X;
    int y = FRAME_Y;
    DrawRectangle(FRAME_X, FRAME_Y, FRAME_WIDTH, FRAME_HEIGHT, BLACK);
    for (int i = 0; i < ENTRY_COUNT; i += 1)
    {
        if (!entries[i].present)
            continue;
        bool mouse_in = CheckCollisionPointRec(
            GetMousePosition(),
            {float(x+PADDING), float(y+PADDING), RECT_SIZE, RECT_SIZE}
        );
        if (picked == i)
            DrawRectangle(x, y, OVERALL, OVERALL, ColorAlpha(WHITE, 0.4));
        else if (mouse_in)
            DrawRectangle(x, y, OVERALL, OVERALL, ColorAlpha(WHITE, 0.2));

        DrawRectangle(x+PADDING, y+PADDING, RECT_SIZE, RECT_SIZE, tileset.get_color(entries[i].value));
        x += OVERALL;
    }
}
