#include <raylib.h>
#include <vector>
#include "ball.h"

const int screenWidth = 800;
const int screenHeight = 600;

const int mode = 0;

int numBalls = 100;
const int minRadius = 5;
const int maxRadius = 10;
const int minSpeed = 1;
const int maxSpeed = 5;
const int minLife = 100;
const int maxLife = 200;

float frameTimeAcc;
const float eraTimeLimit = 3.0f;
bool isInEndGamePause = false;
float currentPauseTime = .0f;
float pauseTimeLimit = 2.0f;

const std::vector<Color> myColors =
{
    LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE,
    BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE
};

std::vector<Ball> myBalls;


void createBalls()
{
    //numBalls = GetRandomValue(10, 50);
    myBalls.clear();
    for (int i = 0; i < numBalls; i++) {
        Ball ball;
        ball.Randomize();

        for (auto&& otherBall : myBalls)
        {
            while (CheckCollisionCircles(Vector2{ (float)ball.x, (float)ball.y }, ball.radius, Vector2{ (float)otherBall.x, (float)otherBall.y }, otherBall.radius))
            {
                ball.Randomize();
            }
        }
        myBalls.push_back(ball);
    }
}

void Update()
{
    if (IsKeyPressed(KEY_SPACE)) {
        createBalls();
    }

    if (isInEndGamePause) {
        if (currentPauseTime < pauseTimeLimit) {
            currentPauseTime += GetFrameTime();
            return;
        }
        currentPauseTime = 0.0f;
        frameTimeAcc = 0.0f;
        isInEndGamePause = false;
        createBalls();
    }

    frameTimeAcc += GetFrameTime();

    if (frameTimeAcc >= eraTimeLimit)
    {
        frameTimeAcc = 0.0f;
        isInEndGamePause = true;
        currentPauseTime = 0.0f;
        //createBalls();
    }

    if (mode) {
        if (myBalls.size() == 1) {
            if (!isInEndGamePause) {
                isInEndGamePause = true;
                currentPauseTime = 0.0f;
            }

        }
    }


    // check collisions and lifetime
    for (auto it1 = myBalls.begin(); it1 < myBalls.end(); it1++) {

        Ball& ball1 = *it1;
        if (frameTimeAcc > ball1.GetLifeTime()) {
            myBalls.erase(it1);
            continue;
        }

        for (auto it2 = myBalls.begin(); it2 < myBalls.end(); it2++) {

            Ball& ball2 = *it2;
            if (ball1 == ball2) {
                continue;
            }

            if (CheckCollisionCircles(Vector2{ (float)ball1.x, (float)ball1.y }, ball1.radius, Vector2{ (float)ball2.x, (float)ball2.y }, ball2.radius))
            {
                static const float scaleRad = 0.5f;

                if (mode) {
                    if (ball1.radius > ball2.radius) {
                        myBalls.erase(it2);
                        ball1.radius += ball2.radius * scaleRad;
                    }
                    else {
                        myBalls.erase(it1);
                        ball1.radius += ball2.radius * scaleRad;
                    }
                }
                else {
                    ball1.Reflect();
                    ball2.Reflect();
                }
            }
        }
    }
}

void DrawDebugInfo() {
    const int fontSize = 20;
    const int startX = screenWidth / 20;
    const int startY = screenHeight / 20;

    DrawText(TextFormat("Balls: %d", numBalls), startX, startY, fontSize, BLACK);
    DrawText(TextFormat("Time Left: %.2f", eraTimeLimit - frameTimeAcc), startX, startY + fontSize + 10, fontSize, BLACK);
    DrawText(TextFormat("Is in pause: %d", isInEndGamePause), startX, startY + 2 * (fontSize + 10), fontSize, BLACK);
    DrawText(TextFormat("Current pause time: %.2f", currentPauseTime), startX, startY + 3 * (fontSize + 10), fontSize, BLACK);
}

int main()
{
    Color darkGreen = Color{ 20, 160, 133, 255 };


    InitWindow(screenWidth, screenHeight, "PhysBall demo");
    SetTargetFPS(60);

    createBalls();

    while (!WindowShouldClose())
    {
        Update();

        BeginDrawing();
        ClearBackground(darkGreen);



        for (auto& ball : myBalls) {
            if (isInEndGamePause == false) {
                ball.Update();
            }
            ball.Draw();
        }

        DrawDebugInfo();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}