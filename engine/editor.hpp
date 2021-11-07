#include <ext/raylib.h>
#include <ui/color_picker.hpp>
#include <engine/tile_editor.hpp>
#include <engine/tileset.hpp>
#include <core/dispatch.hpp>
#include <engine/tile_picker.hpp>
#include <ui/ux.hpp>
#include <memory.h> 
#include <cstdio>

struct Editor 
{
    TilemapEditor editor = {};
    Tileset tileset;
    int frames_passed = 0;
    Ux ux {};
    TilePicker picker;
    Ui::Panel *root;
    UiColorPicker color_picker {};
    Ui::Panel *picker_panel;
    Editor();
    ~Editor();
    
    void
    tick();
    
private:
    void setup_ui();
    void load();
    void save();
};
