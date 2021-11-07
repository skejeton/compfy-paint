#include "color_picker.hpp"
#include <cstdlib>
#include <cmath>
#include <cstdio>
#define PIXEL_BOUNDS 512

UiColorPicker::UiColorPicker()
{
    Image hsv_gradient;
    hsv_gradient.width = PIXEL_BOUNDS;
    hsv_gradient.height = PIXEL_BOUNDS;
    hsv_gradient.mipmaps = 1;
    hsv_gradient.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
    // Multiplying by 3 because BPP is 24 
    hsv_gradient.data = malloc(hsv_gradient.width*hsv_gradient.height*3);
    char *data = (char*)hsv_gradient.data;
    int i = 0;
    for (int x = 0; x < hsv_gradient.width; x += 1)
        for (int y = 0; y < hsv_gradient.height; y += 1)
        {
            float hue = (y/(float)hsv_gradient.width)*360.0f;
            float saturation = x/(float)hsv_gradient.height;
            Color color = ColorFromHSV(hue, 1.0-saturation, 1.0);
            data[i++] = color.r;
            data[i++] = color.g;
            data[i++] = color.b;
        }
    hue_sat = LoadTextureFromImage(hsv_gradient);
    
    // The texture is now in GPU, we can free the image
    free(hsv_gradient.data);
}

UiColorPicker::~UiColorPicker()
{
    UnloadTexture(hue_sat);
}

void
UiColorPicker::set_color(Color color)
{
    Vector3 hsv = ColorToHSV(color);
    printf("%g %g %g\n", hsv.x, hsv.y, hsv.z);
    
    this->hue_sat_x = hsv.x/360.0*PIXEL_BOUNDS;
    this->hue_sat_y = (1.0-hsv.y)*PIXEL_BOUNDS;
    this->value = hsv.z;
}

// Returns the requested color you picked
Color
UiColorPicker::get_color()
{
    float hue = (hue_sat_x/(float)PIXEL_BOUNDS)*360.0;
    float sat = 1.0-(hue_sat_y/(float)PIXEL_BOUNDS);
    float val = value;
    
    return ColorFromHSV(hue, sat, val);
}

#ifndef M_E
#define M_E 2.71828
#endif


// Draw the gizmo
void
UiColorPicker::draw(recti rect)
{
    float x = rect.x(),
         y = rect.y(),
         width = rect.w(),
         height = rect.h();

    Vector2 mouse_pos = GetMousePosition();
    mouse_pos.x -= x;
    mouse_pos.y -= y;
    
    float wr = width/((float)PIXEL_BOUNDS+10+64);
    float hr = height/(float)PIXEL_BOUNDS;
    add -= 1;
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
        && mouse_pos.x > 0 && mouse_pos.y > 0
        && mouse_pos.x < PIXEL_BOUNDS*wr && mouse_pos.y < PIXEL_BOUNDS*hr)
    {
        drag_mode = HUESAT;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
        && mouse_pos.x > (PIXEL_BOUNDS+10)*wr && mouse_pos.y > 0
        && mouse_pos.x < (PIXEL_BOUNDS+10+64)*wr && mouse_pos.y < PIXEL_BOUNDS*hr) 
    {
        drag_mode = VALUE;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        drag_mode = NONE;
    }
    
    if (drag_mode == HUESAT) 
    {
        hue_sat_x = math::clamp(mouse_pos.x, 0.0f, PIXEL_BOUNDS*wr)/wr;
        hue_sat_y = math::clamp(mouse_pos.y, 0.0f, PIXEL_BOUNDS*hr)/hr;
        add += 2;
    }
    add = add < 0 ? 0 : (add > 30 ? 30 : add);
    int old_add = add;
    auto sigmoid = [](float x){ return 1/(1+pow(M_E, -8*(x-0.5))); };
    int add = sigmoid(old_add/30.0f)*30.0f;
    if (drag_mode == VALUE)
    {
        value = 1.0-(math::clamp(mouse_pos.y, 0.0f, PIXEL_BOUNDS*hr)/PIXEL_BOUNDS)/hr;
    }
    unsigned char tint = (char)(value * 255.0);
    DrawTexturePro(hue_sat, { 0, 0, PIXEL_BOUNDS, PIXEL_BOUNDS }, { x, y, PIXEL_BOUNDS*wr, PIXEL_BOUNDS*hr }, {0, 0}, 0, { tint, tint, tint, 255 });
    DrawRectangleGradientV(x+PIXEL_BOUNDS*wr+10*wr, y, 64*wr, PIXEL_BOUNDS*hr, WHITE, BLACK);
    DrawRectangle(x+PIXEL_BOUNDS*wr+5*wr-1, y+(1.0-value)*PIXEL_BOUNDS*hr-1-3, 74*wr+2, 5+3, { 100, 100, 100, 100 });
    DrawRectangle(x+PIXEL_BOUNDS*wr+5*wr, y+(1.0-value)*PIXEL_BOUNDS*hr-3, 74*wr, 5, { 20, 20, 20, 255 });
    DrawCircle(hue_sat_x*wr+x, hue_sat_y*hr+y, 13+add, { 100, 100, 100, 100 });
    DrawCircle(hue_sat_x*wr+x, hue_sat_y*hr+1+y, 13+add, { 100, 100, 100, 100 });
    DrawCircle(hue_sat_x*wr+x, hue_sat_y*hr+y, 12+add, { 50, 50, 50, 255 });
    DrawCircle(hue_sat_x*wr+x, hue_sat_y*hr+y, 11+add, get_color());
}

