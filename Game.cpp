#include "raylib.h"

// Define the game states
enum GameScreen { TITLE, GAME, PAUSE, VICTORY, INSTRUCTIONS, QUIT };

// Player settings
struct Player {
    Rectangle rect;
    Vector2 speed;
    bool onGround;
};

const int screenWidth = 800;
const int screenHeight = 600;
const float GRAVITY = 0.5f;  // Gravity force
const float JUMP_STRENGTH = -10.0f;  // Jumping force

int main() {
    // Initialization
    InitWindow(screenWidth, screenHeight, "Platformer Game");

    GameScreen currentScreen = TITLE;

    // Menu item positions
    int startGamePosY = 200;
    int instructionsPosY = 280;
    int quitPosY = 360;

    // Font settings
    int fontSize = 20;

    // Player setup
    Player player = {{400, 500, 40, 60}, {0, 0}, true};  // starting at the bottom center of the screen

    // Level platforms
    Rectangle platforms[] = {
        {100, 500, 200, 20},  // platform 1
        {300, 400, 200, 20},  // platform 2
        {500, 300, 200, 20},  // platform 3
        {200, 150, 200, 20},  // platform 4
    };

    int totalPlatforms = sizeof(platforms) / sizeof(platforms[0]);

    // Game loop
    while (!WindowShouldClose()) {
        // Get mouse position
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // Check for clickable areas (menu items)
        bool isStartGameHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= startGamePosY && mouseY <= startGamePosY + fontSize);
        bool isInstructionsHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= instructionsPosY && mouseY <= instructionsPosY + fontSize);
        bool isQuitHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= quitPosY && mouseY <= quitPosY + fontSize);

        // Player movement
        if (currentScreen == GAME) {
            // Player movement
            if (IsKeyDown(KEY_D)) {
                player.speed.x = 1.0f;  // move right
            } else if (IsKeyDown(KEY_A)) {
                player.speed.x = -5.0f;  // move left
            } else {
                player.speed.x = 0;
            }

            // Jumping
            if (player.onGround && IsKeyPressed(KEY_SPACE)) {
                player.speed.y = JUMP_STRENGTH;  // jump
                player.onGround = false;
            }

            // Apply gravity
            player.speed.y += GRAVITY;

            // Update player position
            player.rect.x += player.speed.x;
            player.rect.y += player.speed.y;

            // Collision with the ground or platforms
            player.onGround = false;
            for (int i = 0; i < totalPlatforms; i++) {
                if (CheckCollisionRecs(player.rect, platforms[i])) {
                    player.onGround = true;
                    player.rect.y = platforms[i].y - player.rect.height;
                    player.speed.y = 0;
                    break;
                }
            }

            // If player falls below the screen, reset to the start
            if (player.rect.y > screenHeight) {
                player.rect.y = 500;  // reset position
                player.speed.y = 0;
            }
        }

        // Initialization
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen) {
            case TITLE: {
                // Draw Title
                DrawText("LION'S HEART", 200, 100, 60, RED);

                DrawText("Start Game", 250, startGamePosY, 50, isStartGameHovered ? RED : RAYWHITE);
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
                // Draw Platforms
                for (int i = 0; i < totalPlatforms; i++) {
                    DrawRectangleRec(platforms[i], GRAY);
                }

                // Draw Player
                DrawRectangleRec(player.rect, BLUE);

                // Pause the game with "P" key
                if (IsKeyPressed(KEY_P)) {
                    currentScreen = PAUSE; // Switch to PAUSE screen
                }
                break;
            }
            case PAUSE: {
                // Pause screen with option to resume or quit
                DrawText("Game Paused", 250, 100, 30, RAYWHITE);
                DrawText("Press 'R' to Resume", 250, 180, 20, RAYWHITE);
                DrawText("Press 'Q' to Quit", 250, 220, 20, RAYWHITE);

                if (IsKeyPressed(KEY_R)) {
                    currentScreen = GAME; // Resume the game
                }
                if (IsKeyPressed(KEY_Q)) {
                    currentScreen = QUIT; // Quit the game
                }
                break;
            }
            case QUIT: {
                CloseWindow(); // Close the window and exit the game
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
