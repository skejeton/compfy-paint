#pragma once

#include "engine/viewport.hpp"
#include <cstddef>
#include <engine/window_frame.hpp>

struct Ui
{
    struct Dimension
    {
        enum Unit 
        {
            PIXEL,
            RELATIVE,
            AUTO
        } unit;
        float
        get_rel()
        {
            return unit == RELATIVE ? value : 0.0;
        }
        float value;
    };

    struct Style {
        enum Display {
            ROW,
            COLUMN,
            FLOW
        } display;
        int margin = 0;
        Dimension width;
        Dimension height;
        Color color = { 0, 0, 0, 0 };
    };

    struct Panel
    {
        Panel *first_child = NULL;
        Panel *sibling = NULL;

        int real_width = 0;
        int real_height = 0;
        int real_x = 0;
        int real_y = 0;

        bool root = true;
        Style style;
        Viewport viewport()
        {
            return { real_x, real_y, real_width, real_height };
        }
        bool present = false;
        bool is_mouse_over = false;
        bool is_pressed;

        void *self = NULL;
        void *self2 = NULL;
        void (*draw)(void *self) = NULL;
        void (*on_press)(void *self) = NULL;
    };

    Panel *panels;

    Ui();

    ~Ui();

    void
    attach(Panel *parent, Panel *child);

    Panel*
    panel(Style style);

    void
    update();

    void
    draw();
};
