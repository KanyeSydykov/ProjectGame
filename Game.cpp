#include "raylib.h"

enum GameScreen { TITLE, GAME, PAUSE, VICTORY, INSTRUCTIONS, QUIT };

int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Platformer Game");

    GameScreen currentScreen = TITLE;

    // Menu item positions
    int startGamePosY = 200;
    int instructionsPosY = 280;
    int quitPosY = 360;

    // Font settings
    int fontSize = 20;


    // Game loop
    while (!WindowShouldClose()) {
        // Get mouse position
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // Check for clickable areas (menu items)
        bool isStartGameHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= startGamePosY && mouseY <= startGamePosY + fontSize);
        bool isInstructionsHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= instructionsPosY && mouseY <= instructionsPosY + fontSize);
        bool isQuitHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= quitPosY && mouseY <= quitPosY + fontSize);

        // Initialization
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen) {
            case TITLE: {
                // Draw Title
                DrawText("LION'S HEART", 200, 100, 60, RED);

                DrawText("Start Game", 250, startGamePosY, 50, isStartGameHovered ? RED :RAYWHITE);
                DrawText("Instructions", 250, instructionsPosY, 50, isInstructionsHovered ? RED : RAYWHITE);
                DrawText("Quit", 250, quitPosY, 50, isQuitHovered ? RED : RAYWHITE);

                // Handle mouse click for navigation
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (isStartGameHovered) currentScreen = GAME;
                    if (isInstructionsHovered) currentScreen = INSTRUCTIONS;
                    if (isQuitHovered) currentScreen = QUIT;
                }
                break;
            }
            case INSTRUCTIONS: {
                // Draw Instructions Screen
                DrawText("Instructions:", 200, 100, 30, BLACK);
                DrawText("1. Use WASD keys to move.", 200, 150, 20, GRAY);
                DrawText("2. Collect items to progress.", 200, 200, 20, GRAY);
                DrawText("3. Capture the flag!", 200, 250, 20, GRAY);
                DrawText("Press SPACE to return to the title screen.", 200, 400, 20, GRAY);

                // Return to Title Screen
                if (IsKeyPressed(KEY_SPACE)) currentScreen = TITLE;
                break;
            }
            case GAME: {
                // Placeholder for game logic
                DrawText("Game is running!", 250, 250, 20, GRAY);
                break;
            }
            case QUIT: {
                CloseWindow();
                return 0; // Exiting from "quit"
            }
            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
