#include "ux.hpp"

static void draw_button_widget(void *self___)
{
    auto *self_widget = (Ux::Widget*)self___;
    auto *self = self_widget->panel;

    Rectangle rec = {(float)self->real_x, (float)self->real_y, (float)self->real_width, (float)self->real_height};
    float fac = 10/(float)rec.width;
    rec.x -= 1;
    rec.y -= self->is_pressed ? 2 : 1;
    rec.width += 2;
    rec.height += 4;
    DrawRectangleRounded(rec, fac, 5, { 10, 10, 10, 64 });
    rec.x += 1;
    rec.y += self->is_pressed ? 3 : 1;
    rec.width -= 2;
    rec.height -= 3;
    Rectangle cpy = rec;
    cpy.x -= 1;
    cpy.y -= 1;
    cpy.width += 2;
    cpy.height += 2;

    DrawRectangleRounded(cpy, fac, 5, { 100, 100, 100, self->is_pressed ? (unsigned char)5 : (unsigned char)70 });
    DrawRectangleRounded(rec, fac, 5, { 30, 30, 30, self->is_pressed ? (unsigned char)100 : (unsigned char)160 });
    float ofs = (rec.width-MeasureText(self_widget->txt, 10))/2.0;
    DrawText(self_widget->txt, rec.x+ofs, rec.y+10, 10, ColorAlpha(RAYWHITE, 0.8));
}

Ux::Ux()
{
    widgets = new Widget[1024];
}

Ux::~Ux()
{
    delete[] widgets;
}

static Ui::Style default_styles[] = {
    Ui::Style {
        Ui::Style::ROW,
        5,
        { Ui::Dimension::PIXEL, 60 },
        { Ui::Dimension::RELATIVE, 1.0},
    }
};

static Ux::Widget* find_free_widget(Ux *ui)
{
    for (int i = 0; i < 1024; i += 1) 
    {
        if (!ui->widgets[i].present)
        {
            return &ui->widgets[i];
        }
    }
    return nullptr;
}

Ux::Widget*
Ux::simple_button(const char *text)
{
    Ui::Panel *p;
    p = ui.panel(default_styles[0]);
    auto widget = find_free_widget(this);
    p->self = widget;
    p->draw = draw_button_widget;
    widget->kind = SIMPLE_BUTTON;
    widget->txt = text;
    widget->present = true;
    widget->panel = p;
    return widget;
}

void
Ux::draw()
{
    ui.draw();
}

void
Ux::update()
{
    ui.update();
}
