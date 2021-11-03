// User experience (widgets basically)
#pragma once
#include "ui.hpp"

struct Ux
{
    enum Type
    {
        WHATEVER,
        SIMPLE_BUTTON,
    };

    struct Widget 
    {
        bool present = false;
        Ui::Panel *panel = NULL;
        Type kind = WHATEVER;
        const char *txt = NULL;
    };

    Widget *widgets;

    Ui ui;
    Ux();
    ~Ux();

    Widget*
    simple_button(const char *text);

    void
    update();

    void 
    draw();
};
