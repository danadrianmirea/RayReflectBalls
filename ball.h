#pragma once
#include <vector>
#include <raylib.h>
#include "globals.h"

class Ball
{
public:
    Ball();

    bool operator == (const Ball& other) {
        if (x != other.x) return false;
        if (y != other.y) return false;
        return true;
    }


    void SetColor(Color c);
    void SetPosition(Vector2 pos) {
        x = pos.x;
        y = pos.y;
    }
    void SetMoveDirection(Vector2 dir) {
        speedX = dir.x;
        speedY = dir.y;
    }
    void SetRadius(int r) {
        radius = r;
    }

    void SetLifeTime(int t) {
        lifeTime = t;
    }

    inline int GetLifeTime() {
        return lifeTime;
    }

    void Reflect()
    {
        speedX *= -1;
        speedY *= -1;
        x += speedX;
        y += speedY;
    }

    void Randomize()
    {
        SetColor(myColors[GetRandomValue(0, myColors.size())]);
        int radius = GetRandomValue(minRadius, maxRadius);
        SetRadius(radius);
        Vector2 randomPos = Vector2{ (float)GetRandomValue(radius * 2.0f, screenWidth - radius * 2.0f), GetRandomValue(radius * 2.0f, screenHeight - radius * 2.0f) };
        SetPosition(randomPos);
        int speedX = GetRandomValue(minSpeed, maxSpeed);
        int sign = GetRandomValue(0, 1);
        if (!sign) speedX *= -1;
        int speedY = GetRandomValue(minSpeed, maxSpeed);
        sign = GetRandomValue(0, 1);
        if (!sign) speedY *= -1;
        Vector2 dir = Vector2{ (float)speedX, (float)speedY };
        SetMoveDirection(dir);
        int life = GetRandomValue(minLife, maxLife);
        SetLifeTime(life);
    }

    void Update();
    void Draw();

public:
    Color color;
    int x;
    int y;
    int speedX;
    int speedY;
    int radius;
    int lifeTime;
private:
    float frameTime;
};