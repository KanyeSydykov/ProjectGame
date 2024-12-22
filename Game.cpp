#include "raylib.h"

int main()
{
    InitWindow(800, 450, "Lion's Heart");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("LION'S HEART", GetScreenWidth() / 2 - MeasureText("LION'S HEART", 40) / 2, GetScreenHeight() / 2 - 40, 40, RED);
        DrawText("PRESS ANY BUTTON", GetScreenWidth() / 2 - MeasureText("PRESS ANY BUTTON", 20) / 2, GetScreenHeight() / 2 + 20, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}