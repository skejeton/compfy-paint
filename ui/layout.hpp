#include <core/math.hpp>

struct LayoutFlex
{
    int primary = 0;
    int secondary = 0;
    float rel = 0;

    void
    push(int pr, int sc, float rel)
    {
        primary += pr;
        this->rel += rel;
        secondary = math::max(secondary, sc);
    }

    float
    multiplier(float fac)
    {
        return fac / rel;
    }
};

struct LayoutWrap
{
    int bound = 0;
    int x = 0, y = 0;
    int row_h = 0;

    LayoutWrap(int bound)
    {
        this->bound = bound;
    }

    bool
    push(int w, int h)
    {
        bool check = x != 0;
        x += w;
        row_h = math::max(row_h, h);
        if (x > bound)
        {
            x = 0;
            y += row_h;
            row_h = 0;
            return check;
        }
        return false;
    }
};
