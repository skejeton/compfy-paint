#pragma once

namespace math {

template <class T> T
max(T a, T b)
{
    return a > b ? a : b;
}

template <class T> T
min(T a, T b)
{
    return a < b ? a : b;
}

template <class T> T
clamp(T a, T l, T u)
{
    return min(max(a, l), u);
}

template <class T>
struct Vec2
{
    T x;
    T y;

    Vec2()
    {
        this->x = 0;
        this->x = 0;
    }

    Vec2(Vector2 v)
    {
        this->x = v.x;
        this->y = v.y;
    }

    Vec2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    Vec2<double> floor() {
        return {
            floor(this->x),
            floor(this->y),
        };
    }

    bool operator==(const Vec2 &rhs) const 
    {
        return this->x == rhs.x && this->y == rhs.y; 
    }

    bool operator<(const Vec2 &rhs) const 
    {
        return this->mag() < rhs.mag();
    }

    const Vec2<T> operator+(const Vec2<T> other) const
    {
        return Vec2(this->x + other.x, this->y + other.y);
    }

    Vec2<T>& operator+=(const Vec2<T> other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vec2<T>& operator-=(const Vec2<T> other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    const Vec2<T> operator-(const Vec2<T> other) const
    {
        return Vec2(this->x - other.x, this->y - other.y);
    }

    Vec2<T> operator*(Vec2<T> other) const
    {
        return Vec2(this->x * other.x, this->y * other.y);
    }

    Vec2<T> operator/(Vec2<T> other) const
    {
        return Vec2(this->x / other.x, this->y / other.y);
    }

    Vec2<T>& operator*=(const T &other) 
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    Vec2<T>& operator/=(const T &other) 
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    template <class X>
    operator Vec2<X>() const 
    {
        return Vec2<X> (
            static_cast<X>(this->x),
            static_cast<X>(this->y)
        );
    }

    Vec2<double> real() const {
        return static_cast<Vec2<double>>(*this);
    }

    Vec2<T> operator*(T other) const
    {
        return Vec2(this->x * other, this->y * other);
    }

    Vec2<T> operator/(T other) const
    {
        return Vec2(this->x / other, this->y / other);
    }

    Vec2<T> operator%(T other) const
    {
        return Vec2(this->x % other, this->y % other);
    }

    Vec2<T> operator-() const
    {
        return Vec2(-x, -y);
    }

    T mag() const
    {
        return sqrt(x*x + y*y);
    }

    Vec2<T> norm() const
    {
        auto mag = this->mag();
        if (mag != 0)
            return *this / mag;
        return Vec2(0, 0);
    }

    Vector2 toRaylib() const {
        return { (float)this->x, (float)this->y };
    }
};

template <class T>
struct Rect
{
    Vec2<T> pos;
    Vec2<T> size;

    static Rect<T> pixel(Vec2<T> pos) {
        return Rect(pos, { 1, 1 });
    }

    Rect() {
    
    }
    
    T& x() 
    {
        return pos.x;
    }


    T& y() 
    {
        return pos.y;
    }
    
    T& w() 
    {
        return size.x;
    }
    
    T& h() 
    {
        return size.y;
    }

    Vec2<T> middle() {
        return this->pos+this->size/2;
    }

    Rect(T x, T y, T width, T height)
    {
        this->pos = Vec2<T>(x, y);
        this->size = Vec2<T>(width, height);
    }

    Rect<T> diff(Rect<T> other)
    {
        auto x = max(this->pos.x, other.pos.x);
        auto y = max(this->pos.y, other.pos.y);
        return Rect<T>(
            x,
            y,
            min(this->pos.x+this->size.x, other.pos.x+other.size.x)-x,
            min(this->pos.y+this->size.y, other.pos.y+other.size.y)-y
        );
    }

    template <class X>
    operator Rect<X>() const 
    {
        return Rect<X> (
            static_cast<X>(this->pos),
            static_cast<X>(this->size)
        );
    }


    Rect(Vec2<T> pos, Vec2<T> size)
    {
        this->pos = pos;
        this->size = size;
    }

    Rect<T> floor() {
        return {
            pos.floor(),
            size.floor()
        };
    }

    bool intersects(Rect<T> other)
    {
        return pos.x + size.x > other.pos.x &&
               pos.x < other.pos.x + other.size.x &&
               pos.y + size.y > other.pos.y &&
               pos.y < other.pos.y + other.size.y;
    }

    Rectangle toRaylib() const {
        return Rectangle {
            (float)this->pos.x,
            (float)this->pos.y,
            (float)this->size.x,
            (float)this->size.y,
        };
    }

    Rect<T> at(Vec2<T> pos)
    {
        return Rect(this->pos + pos, this->size);
    }
};


}

// Vector of 2 floats
typedef math::Vec2<double> vf2;
// Vector of 2 ints
typedef math::Vec2<int> vi2;
// Rect of doubles
typedef math::Rect<double> rectf;
// Rect of ints
typedef math::Rect<int> recti;
