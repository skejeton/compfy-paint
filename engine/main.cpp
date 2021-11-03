/*
 * This file is a part of Comfy Engine
 * (C) 2021
 */
#include <cstdio>
extern "C" {
#include <ext/raylib.h>
}
#include <engine/tile_editor.hpp>
#include <engine/tileset.hpp>
#include <engine/tile_picker.hpp>
#include <ui/ux.hpp>

int
main()
{
    static TilemapEditor editor = { 32, 32, 0, 0, 16, 16 };
    TilePicker picker;
    editor.selected_tile = 1;

    static Tileset tileset;
    picker.add_tile(1);
    picker.add_tile(2);
    picker.add_tile(3);
    picker.add_tile(4);
    picker.add_tile(5);
    picker.add_tile(6);
    picker.add_tile(7);
    picker.add_tile(8);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1920/2, 1080/2, "compfy engine");
    static auto font = LoadFontEx("Roboto-Regular.ttf", 16, NULL, 0);

    Ux ux {};

    Ui &ui = ux.ui;
    Ui::Panel *root = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .width = { Ui::Dimension::PIXEL, (float)GetScreenWidth() },
        .height = { Ui::Dimension::PIXEL, (float)GetScreenHeight() },
    });
    editor.panel = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::RELATIVE, 1.0 },
    });
    auto draw2 = [](void *self___){
        auto *self = (Ui::Panel*)self___;
        DrawRectangle(self->real_x-16, self->real_y, 16, self->real_height, { 0, 0, 0, 32 });
        DrawRectangle(self->real_x-8, self->real_y, 8, self->real_height, { 0, 0, 0, 32 });
        DrawRectangle(self->real_x-4, self->real_y, 4, self->real_height, { 0, 0, 0, 32 });
        DrawRectangle(self->real_x-2, self->real_y, 2, self->real_height, { 0, 0, 0, 32 });
        DrawRectangle(self->real_x, self->real_y, 1, self->real_height, { 255, 255, 255, 16 });
    };
    static auto test = ui.panel(Ui::Style {
        .display = Ui::Style::COLUMN,
        .width = { Ui::Dimension::RELATIVE, 0.5 },
        .height = { Ui::Dimension::RELATIVE, 1.0 },
        .color = { 32, 32, 32, 32 }
    });
    test->self = test;
    test->draw = draw2;
    auto draw = [](void *self___){
        auto *self = (Ui::Panel*)self___;

    };
    Ui::Panel *items;
    ui.attach(test, items = ui.panel(Ui::Style {
        .display = Ui::Style::FLOW,
        .margin = 5,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::RELATIVE , 1.0 },
    }));
    Ui::Panel *x;
    ui.attach(test, x = ui.panel(Ui::Style {
        .display = Ui::Style::ROW,
        .margin = 5,
        .width = { Ui::Dimension::RELATIVE, 1.0 },
        .height = { Ui::Dimension::PIXEL, 40 },
    }));

    Ui::Panel *p;

    ui.attach(x, ux.simple_button("Load")->panel);
    ui.attach(x, ux.simple_button("Save")->panel);
    auto bruh = "";
    auto on_press = [](void *self___){
        auto *self = (Ui::Panel*)self___;
        auto color = self->first_child->style.color;
        for (int z = 0; z < 100; z += 1)
        {
            auto a = tileset.tiles[z].color;
            auto b = color;
            if (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a)
            {
                editor.selected_tile = z+1;
                printf("Color %d %d %d %d\n", color.r, color.g, color.b, color.a);
            }
        }
    };
    int i = 0;
    auto add_color = [&](Color color){
        tileset.tiles[i] = { true, color };
        i += 1;

        auto pan = ux.simple_button("")->panel; 
        pan->style = Ui::Style {
            .display = Ui::Style::ROW,
            .margin = 5,
            .width = { Ui::Dimension::AUTO, 100 },
            .height = { Ui::Dimension::AUTO, 100 },
        };
        
        
        pan->self2 = pan;
        pan->on_press = on_press;

        ui.attach(items, pan);

        ui.attach(pan, ui.panel(Ui::Style {
            .display = Ui::Style::ROW,
            .margin = 10,
            .width = { Ui::Dimension::PIXEL, 50 },
            .height = { Ui::Dimension::PIXEL, 50 },
            .color = color
        }));
    };

    add_color(RED);
    add_color(GREEN);
    add_color(BLUE);
    add_color(ORANGE);
    add_color(PURPLE);
    add_color(RAYWHITE);
    add_color(BLACK);
    add_color(BROWN);


    ui.attach(root, editor.panel);
    ui.attach(root, test);


    // Disable exit on ESC
    SetExitKey(0);
    SetTargetFPS(100);            
    while (!WindowShouldClose())
    {
        root->style.width.value = (float)GetScreenWidth(); 
        root->style.height.value = (float)GetScreenHeight(); 
        BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        //if (!picker.update())
        editor.update();
        ClearBackground(Color { 16, 16, 16, 255 });
        editor.draw(tileset);
        ui.update();
        ux.draw();
        //picker.draw(tileset);
        EndBlendMode();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

