#include "vector.h"

#include <math.h>
#include <stdio.h>

const float PI = 3.1415926;

Vec2 Vector2(float x, float y)
{
    Vec2 new;

    new.x = x;
    new.y = y;

    return new;
}

void Vector2_Print(Vec2 v)
{
    printf("[%.2f, %.2f]\n", v.x, v.y);
}

Vec2 Vector2_Add(Vec2 a, Vec2 b)
{
    Vec2 new;

    new.x = a.x + b.x;
    new.y = a.y + b.y;

    return new;
}

Vec2 Vector2_Sub(Vec2 a, Vec2 b)
{
    Vec2 new;

    new.x = a.x - b.x;
    new.y = a.y - b.y;

    return new;
}

Vec2 Vector2_Mul(Vec2 v, float n)
{
    Vec2 new;

    new.x = v.x * n;
    new.y = v.y * n;

    return new;
}

Vec2 Vector2_Div(Vec2 v, float n)
{
    Vec2 new;

    new.x = v.x / n;
    new.y = v.y / n;

    return new;
}

float Vector2_Dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

float Vector2_Mag(Vec2 v)
{
    return (float) sqrt(v.x * v.x + v.y * v.y);
}

Vec2 Vector2_Norm(Vec2 v)
{
    return Vector2_Div(v, Vector2_Mag(v));
}

float DegreesToRadians(float degrees)
{
    return (degrees * PI) / 180;
}

Vec2 Vector2_Rotate(Vec2 v, float radians)
{
    Vec2 new;

    float cosine = (float) cos(radians);
    float sine = (float) sin(radians);

    new.x = v.x * cosine - v.y * sine;
    new.x = v.x * sine + v.y * cosine;

    return new;
}

float Lerp(float start, float end, float percent)
{
    return start + (end - start)*percent;
}

