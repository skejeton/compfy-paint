#pragma once

#include "engine/viewport.hpp"
#include <cstddef>
#include <engine/window_frame.hpp>
#include <core/dispatch.hpp>

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
        bool hide_overflow = false;
    };

    struct Panel
    {
        Panel *first_child = NULL;
        Panel *sibling = NULL;
        vf2 scroll_vel = { 0, 0 };
        vi2 scroll = { 0, 0 };
        vi2 calc_scroll = { 0, 0 };
        int real_width = 0;
        int real_height = 0;
        int real_x = 0;
        int real_y = 0;

        bool root = true;
        Style style;
        Viewport viewport()
        {
            return {{ real_x + calc_scroll.x, real_y + calc_scroll.y, real_width, real_height }};
        }
        bool present = false;
        bool is_mouse_over = false;
        bool is_pressed = false;

        Selfcall<void> draw;
        Dispatch<> on_click;
    };

    Panel *panels;

    Ui();

    ~Ui();

    void
    attach(Panel *parent, Panel *child);

    Panel*
    panel(Style style);

    void
    destroy(Panel *panel);

    void
    destroy_all_children(Panel *panel);

    void
    update();

    void
    draw();
};
