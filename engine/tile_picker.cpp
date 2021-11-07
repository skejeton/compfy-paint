#include "tile_picker.hpp"
#include "ext/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define ENTRY_COUNT 1024



TilePicker::TilePicker(Ux *ux)
{
    Ui &ui = ux->ui;
    this->ux = ux;
}

static void
on_press(void *self___)
{
    auto *self = (TilePicker::Context*)self___;

    self->picker->picked = self->id;
}

static void
add_color(TilePicker *self, int id, Color color)
{
    auto &ui = self->ux->ui;
    auto pan = self->ux->simple_button("")->panel;
    pan->style = Ui::Style {
        .display = Ui::Style::ROW,
        .margin = 3,
        .width = { Ui::Dimension::AUTO, 0 },
        .height = { Ui::Dimension::AUTO, 0 },
    };
    self->contexts[self->context_count] = { id, self };
    pan->on_click.subscribe(&self->contexts[self->context_count++], on_press);

    ui.attach(self->root, pan);

    ui.attach(pan, ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .margin  = 3,
        .width   = { Ui::Dimension::PIXEL, 40 },
        .height  = { Ui::Dimension::PIXEL, 40 },
        .color   = color
    }));
}

void
TilePicker::use_tileset(Tileset &tileset)
{
    this->context_count = 0;
    ux->ui.destroy_all_children(this->root);
    for (int i = 0; i < 1024; i += 1)
    {
        Tileset::Tile &tile = tileset.tiles[i];
        if (!tile.present)
            continue;

        add_color(this, i+1, tile.color);
    }
    printf("===\n");
}

