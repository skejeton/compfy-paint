#include "ui.hpp"
#include <ui/layout.hpp>
#include <cstdio>
#include <engine/window_frame.hpp>
#include <ext/raylib.h>

#define PANEL_COUNT 1024

Ui::Ui()
{
    panels = new Panel[PANEL_COUNT];
}

Ui::~Ui()
{
    delete[] panels;
}

static Ui::Panel* find_free_panel(Ui *ui)
{
    for (int i = 0; i < PANEL_COUNT; i += 1)
    {
        if (!ui->panels[i].present)
        {
            return &ui->panels[i];
        }
    }
    return nullptr;
}


void
Ui::attach(Ui::Panel *parent, Ui::Panel *child)
{
    child->root = false;
    if (parent->first_child == NULL)
    {
        parent->first_child = child;
    }
    else
    {
        Ui::Panel *p = parent->first_child;
        while (p->sibling) p = p->sibling;
        p->sibling = child;
    }
}

Ui::Panel*
Ui::panel(Ui::Style style)
{
    Panel *panel = find_free_panel(this);
    panel->style = style;
    panel->present = true;
    return panel;
}

static int
initial(Ui::Dimension dim, int parent)
{
    switch (dim.unit)
    {
        case Ui::Dimension::PIXEL:
            return (int)dim.value;
        case Ui::Dimension::RELATIVE:
            return parent;
        case Ui::Dimension::AUTO:
            return 0;
    }
}

struct Size
{
    int width;
    int height;
};

static Size
layout(Ui *self, Ui::Panel *panel, int suggest_w, int suggest_h, int x, int y);

static Size
layout_flow(Ui *self, Ui::Panel *panel, int x, int y)
{
    const bool IS_AUTO = panel->style.width.unit == Ui::Dimension::AUTO;
    int bound = IS_AUTO ? 10000000 : panel->real_width;
    LayoutWrap wrap { bound };
    Ui::Panel *p = panel->first_child;
    for (;p; p = p->sibling)
    {
        Size their = layout(self, p, 0, 0, x+wrap.x, y+wrap.y);
        if (wrap.push(their.width, their.height))
        {
            Size their = layout(self, p, 0, 0, x+wrap.x, y+wrap.y);
            wrap.push(their.width, their.height);
        }
    }

    return { wrap.x, wrap.y+wrap.row_h };
}

static Size
layout_rowcol(Ui *self, Ui::Panel *panel, int x, int y)
{
    const bool IS_ROW = panel->style.display == panel->style.ROW;

    Ui::Panel *p = panel->first_child;

    LayoutFlex flex {};

    int roam = IS_ROW ? panel->real_width : panel->real_height;

    for (;p; p = p->sibling)
    {
        Size their = layout(self, p, 0, 0, 0, 0);

        if (IS_ROW)
            flex.push(their.width, their.height, p->style.width.get_rel());
        else
            flex.push(their.height, their.width, p->style.height.get_rel());
    }

    roam -= flex.primary;


    float prel = flex.rel;
    int primary = flex.primary;
    int secondary = flex.secondary;

    flex = {};

    if (prel == 0) prel = 0.001;
    p = panel->first_child;
    for (;p; p = p->sibling)
    {
        Size their;
        if (IS_ROW)
        {
             their = layout(self, p, roam*p->style.width.get_rel()/prel, panel->real_height,
                x+flex.primary,
                y);
             flex.push(their.width, their.height, 0.0);
        }
        else
        {
             their = layout(self, p, panel->real_width, roam*p->style.height.get_rel()/prel,
                x,
                y+flex.primary);
             flex.push(their.height, their.width, 0.0);
        }
    }

    if (IS_ROW)
        return { flex.primary, flex.secondary };
    else
        return { flex.secondary, flex.primary };
}

static Size
layout(Ui *self, Ui::Panel *panel, int suggest_w, int suggest_h, int x, int y)
{
    suggest_w -= panel->style.margin*2;
    suggest_h -= panel->style.margin*2;
    panel->real_width = initial(panel->style.width, suggest_w);
    panel->real_height = initial(panel->style.height, suggest_h);
    panel->real_x = x+panel->style.margin;
    panel->real_y = y+panel->style.margin;

    Size siz;

    if (panel->first_child == NULL)
        siz = { panel->real_width, panel->real_height };
    else if (panel->style.display == Ui::Style::FLOW)
        siz = layout_flow(self, panel, panel->real_x, panel->real_y);
    else
        siz = layout_rowcol(self, panel, panel->real_x, panel->real_y);

    if (panel->style.width.unit == Ui::Dimension::AUTO)
        panel->real_width = siz.width;
    if (panel->style.height.unit == Ui::Dimension::AUTO)
        panel->real_height = siz.height;
        
    if (siz.height < panel->real_height)
        panel->scroll.y = 0;
    // FIXME: Its kinda bad, scroll should be ideally positive here instead 
    else if (panel->scroll.y < -(siz.height-panel->real_height))
    {
        panel->scroll.y = -(siz.height-panel->real_height);
        panel->scroll_vel.y = 0.0;
    }
    
    return {panel->real_width+panel->style.margin*2,panel->real_height+panel->style.margin*2};
}

void
Ui::destroy(Ui::Panel *panel)
{
    panel->present = false;
    panel->first_child = NULL;
    panel->sibling = NULL;
    for (auto p = panel->first_child; p; p = p->sibling)
    {
        destroy(p);
    }
}

void
Ui::destroy_all_children(Ui::Panel *panel)
{
    panel->first_child = NULL;
    for (auto p = panel->first_child; p; p = p->sibling)
    {
        destroy(p);
    }
}

static void
press_recursive(Ui *ui, Ui::Panel *panel, recti boundary, vi2 pos)
{
    auto pixel = recti::pixel(pos);
    
    if (panel->style.hide_overflow)
        boundary = panel->viewport().rect;
    
    for (auto p = panel->first_child; p; p = p->sibling)
    {
        press_recursive(ui, p, boundary, pos);
    }
    
    if (panel->is_pressed && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        panel->is_pressed = false;
    
    if (pixel.intersects(panel->viewport().rect) && pixel.intersects(boundary))
    {
        float motion = GetMouseWheelMove();
        if (panel->style.hide_overflow)
            panel->scroll_vel.y += motion*300.0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            panel->is_pressed = true;
            panel->on_click();
        }
    }
}

void
Ui::update()
{
    for (int i = 0; i < PANEL_COUNT; i += 1)
    {
        Panel &panel = panels[i];
        if (!panel.present)
            continue;
        panel.scroll.y += panel.scroll_vel.y*GetFrameTime();
        panel.scroll_vel.y /= 1.2;
        panel.scroll.x = math::min(panel.scroll.x, 0);
        panel.scroll.y = math::min(panel.scroll.y, 0);
    }

    for (int i = 0; i < PANEL_COUNT; i += 1)
    {
        Panel &panel = panels[i];
        if (!panel.present)
            continue;
        if (panel.root)
        {
            press_recursive(this, &panel, { 0, 0, GetScreenWidth(), GetScreenHeight() }, { GetMouseX(), GetMouseY() });
            layout(this, &panel, 0, 0, 0, 0);
        }
    }
}


static void 
draw_recursive(Ui *ui, Ui::Panel *panel, vi2 scroll)
{
    panel->calc_scroll = scroll;
    Viewport viewport = panel->viewport();
    
    if (panel->style.hide_overflow)
        BeginScissorMode(viewport.rect.x(), viewport.rect.y(), viewport.rect.w(), viewport.rect.h());
        
    viewport.draw_rectangle(0, 0, panel->real_width, panel->real_height, panel->style.color);
    
    if (panel->draw.valid())
        panel->draw();
        
    scroll += panel->scroll;
    
    for (auto p = panel->first_child; p; p = p->sibling)
    {
        draw_recursive(ui, p, scroll);
    }
    
    if (panel->style.hide_overflow)
        EndScissorMode();
}

void
Ui::draw()
{
    for (int i = 0; i < PANEL_COUNT; i += 1)
    {
        Panel &panel = panels[i];
        if (!panel.present)
            continue;
        if (panel.root)
        {    
            draw_recursive(this, &panel, { 0, 0 });
        }
    }
}
