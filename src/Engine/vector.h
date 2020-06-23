#pragma once


typedef struct Vec2 {
    float x;
    float y;
} Vec2;


Vec2 Vector2(float x, float y);
void Vector2_Print(Vec2 v);

Vec2 Vector2_Add(Vec2 a, Vec2 b);
Vec2 Vector2_Sub(Vec2 a, Vec2 b);
Vec2 Vector2_Mul(Vec2 v, float n);
Vec2 Vector2_Div(Vec2 v, float n);

float Vector2_Dot(Vec2 a, Vec2 b);
float Vector2_Mag(Vec2 v);
Vec2 Vector2_Norm(Vec2 v);

float DegreesToRadians(float degrees);
Vec2 Vector2_Rotate(Vec2 v, float radians);

float Lerp(float start, float end, float percent);
