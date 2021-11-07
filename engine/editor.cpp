#include "editor.hpp"

void draw_shadow_left(void *self___)
{
    auto *self = (Ui::Panel*)self___;
    DrawRectangle(self->real_x-16, self->real_y, 16, self->real_height, { 0, 0, 0, 32 });
    DrawRectangle(self->real_x-8, self->real_y, 8, self->real_height, { 0, 0, 0, 32 });
    DrawRectangle(self->real_x-4, self->real_y, 4, self->real_height, { 0, 0, 0, 32 });
    DrawRectangle(self->real_x-2, self->real_y, 2, self->real_height, { 0, 0, 0, 32 });
    DrawRectangle(self->real_x, self->real_y, 1, self->real_height, { 255, 255, 255, 16 });
}

Editor::Editor()
    : picker(&ux)
{
    tileset = {};
    // Setup default colors
    {
        int i = 0;
        Color colors[] = {
            LIGHTGRAY, GRAY, DARKGRAY, YELLOW,
            GOLD, ORANGE, PINK, RED, MAROON,
            GREEN, LIME, DARKGREEN, SKYBLUE,
            WHITE, BLACK
        };
        for (int i = 0; i < sizeof(colors)/sizeof(Color); i += 1)
        {
            tileset.tiles[i] = { true, colors[i] };
        }
    }
    setup_ui();
    
    this->load();
}

void
Editor::setup_ui()
{
    auto &ui = ux.ui;

    root = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .width = { Ui::Dimension::PIXEL, (float)GetScreenWidth() },
        .height = { Ui::Dimension::PIXEL, (float)GetScreenHeight() },
    });
    editor.panel = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::RELATIVE, 1.0 },
    });
    auto sidebar = ui.panel(Ui::Style {
        .display = Ui::Style::COLUMN,
        .width = { Ui::Dimension::RELATIVE, 0.5 },
        .height = { Ui::Dimension::RELATIVE, 1.0 },
        .color = { 32, 32, 32, 32 }
    });
    
    
    picker.root = ui.panel(Ui::Style {
        .display = Ui::Style::FLOW,
        .margin = 5,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::RELATIVE , 1.0 },
        // .color = RED,
        .hide_overflow = true,
    });
    
    editor.panel->draw = Selfcall<void>(this, [](void *self___){
        auto self = (Editor*)self___;
        self->editor.draw(self->tileset);
    });
    picker.use_tileset(tileset);

    ui.attach(sidebar, picker.root);
    sidebar->draw = {sidebar, draw_shadow_left};
    
    ui.attach(sidebar, picker_panel = ui.panel(Ui::Style {
        .display = Ui::Style::FLOW,
        .margin = 10,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::RELATIVE, 1.0 },
    }));
    Ui::Panel *add_color;
    
    ui.attach(sidebar, add_color = ux.simple_button("Add color")->panel);

    
    add_color->on_click.subscribe(this, [](void *self_){
        auto self = (Editor*)(self_);
        for (int i = 0; i < 1024; i += 1)
        {
            if (!self->tileset.tiles[i].present)
            {
                self->picker.root->scroll_vel.y = -10000;
                self->tileset.tiles[i] = { true, self->color_picker.get_color() };
                self->picker.use_tileset(self->tileset);
                return;
            }
        }
    });
    
    picker.on_color_selected.subscribe(this, [](void *self_, int id){
        auto self = (Editor*)(self_);
        self->color_picker.set_color(self->tileset.get_color(id));
    });
    
    add_color->style.width = { Ui::Dimension::RELATIVE, 1.0 };
    add_color->style.height = { Ui::Dimension::PIXEL, 30 };
    add_color->style.margin = 10;
    
    Ui::Panel *load_save;
    ui.attach(sidebar, load_save = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .margin = 5,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::PIXEL, 40 },
    }));
    editor.panel->style.hide_overflow = true;
    Ui::Panel *load;
    Ui::Panel *save;

    ui.attach(load_save, load = ux.simple_button("Load")->panel);
    ui.attach(load_save, save = ux.simple_button("Save")->panel);

    save->on_click.subscribe(this, [](void *p){ ((Editor*)(p))->save(); });
    load->on_click.subscribe(this, [](void *p){ ((Editor*)(p))->load(); });

    ui.attach(root, editor.panel);
    ui.attach(root, sidebar);

    // Disable exit on ESC
    SetExitKey(0);
    SetTargetFPS(60);

    picker_panel->draw = Selfcall<void>(this, [](void *self___){
        auto self = (Editor*)self___;
        auto viewport = self->picker_panel->viewport();
        self->color_picker.draw(viewport.rect);
    });
}

struct Save {
    uint8_t version;
    Tileset::Tile tiles[1024];
    Tilemap::Pair pairs[PAIR_COUNT];
} __attribute__((packed));

void
Editor::load()
{
    auto &tilemap = editor.tilemap;
    FILE *fh = fopen("painting.pt.cf", "rb");
    if (fh == NULL) return;
    Save save;
    fread(&save, sizeof(Save), 1, fh);
    if (save.version == 1)
    {
        memcpy(tileset.tiles, save.tiles, 1024*sizeof(Tileset::Tile));
        memcpy(tilemap.pairs, save.pairs, PAIR_COUNT*sizeof(Tileset::Tile));
    } 
    else {
        printf("Resorting");
        memcpy(tilemap.pairs, save.pairs, PAIR_COUNT*sizeof(Tileset::Tile));
    }
    picker.use_tileset(tileset);
    fclose(fh);
};

void
Editor::save()
{
    auto &tilemap = editor.tilemap;
    FILE *fh = fopen("painting.pt.cf", "wb");
    if (fh == NULL) return;
    Save save = {};
    save.version = 1;
    memcpy(save.tiles, tileset.tiles, 1024*sizeof(Tileset::Tile));
    memcpy(save.pairs, tilemap.pairs, PAIR_COUNT*sizeof(Tileset::Tile));
    fwrite(&save, sizeof(Save), 1, fh);
    fclose(fh);
};

Editor::~Editor()
{
    this->save();
}

void 
Editor::tick()
{
    if ((frames_passed % 3600) == 0) 
    {
        printf("Autosaving");
        save();
    }
    ux.update();
    
    frames_passed += 1;
    root->style.width.value = (float)GetScreenWidth();
    root->style.height.value = (float)GetScreenHeight();
    BeginDrawing();
    BeginBlendMode(BLEND_ALPHA);
    editor.update();
    ClearBackground(Color { 16, 16, 16, 255 });

    ux.draw();
    
    editor.selected_tile = picker.picked;

    EndBlendMode();
    EndDrawing();
}
