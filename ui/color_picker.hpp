#include <ext/raylib.h>
#include <core/math.hpp>

struct UiColorPicker
{
    enum DragMode
    {
        NONE,
        HUESAT,
        VALUE
    }
    drag_mode = NONE;
    Texture2D hue_sat;
    int add;
    int hue_sat_x = 70, hue_sat_y = 50;
    float value = 0.5;
    
    UiColorPicker();
    ~UiColorPicker();

    // Returns the requested color you picked
    Color
    get_color();
    
    void
    set_color(Color color);
    
    // Draw the gizmo
    void
    draw(recti rect);
};



