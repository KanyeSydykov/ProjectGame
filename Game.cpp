// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "globals.h"
#include "raylib.h"

// Constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 32
#define GRAVITY 980.0f
#define JUMP_FORCE 400.0f
#define MOVE_SPEED 200.0f

// Game screens
enum GameScreen {
    TITLE,
    GAME,
    PAUSE,
    VICTORY,
    INSTRUCTIONS
};

// Structures
struct Player {
    Vector2 position;
    Vector2 velocity;
    bool isJumping;
    int score;
};

struct Coin {
    Vector2 position;
    bool isCollected;
};

struct Level {
    bool* tiles;
    int width;
    int height;
    Vector2 flagPosition;
    Coin* coins;
    int coinCount;
};

// Global variables declarations
extern Player player;
extern Level currentLevel;
extern GameScreen currentScreen;

// Function declarations
void InitLevel();
void UpdateGame(float deltaTime);
void DrawGame();

#endif // GLOBALS_H

// Game.cpp
#include "globals.h"
#include <cstdlib>

// Global variable definitions
Player player;
Level currentLevel;
GameScreen currentScreen = TITLE;

void InitLevel() {
    // Initialize level dimensions
    currentLevel.width = WINDOW_WIDTH / TILE_SIZE;
    currentLevel.height = WINDOW_HEIGHT / TILE_SIZE;

    // Allocate tile map
    currentLevel.tiles = (bool*)malloc(currentLevel.width * currentLevel.height * sizeof(bool));

    // Create level layout
    for (int y = 0; y < currentLevel.height; y++) {
        for (int x = 0; x < currentLevel.width; x++) {
            bool isSolid = false;

            // Create floor
            if (y == currentLevel.height - 1) isSolid = true;

            // Create platforms
            if (y == currentLevel.height - 5 && (x > 5 && x < 12)) isSolid = true;
            if (y == currentLevel.height - 8 && (x > 10 && x < 18)) isSolid = true;

            currentLevel.tiles[y * currentLevel.width + x] = isSolid;
        }
    }

    // Initialize player
    player.position = (Vector2){
        TILE_SIZE * 2.0f,
        static_cast<float>(WINDOW_HEIGHT - TILE_SIZE * 4)
    };
    player.velocity = (Vector2){0.0f, 0.0f};
    player.isJumping = false;
    player.score = 0;

    // Setup coins
    currentLevel.coinCount = 3;
    currentLevel.coins = (Coin*)malloc(currentLevel.coinCount * sizeof(Coin));
    currentLevel.coins[0] = (Coin){{TILE_SIZE * 8.0f, WINDOW_HEIGHT - TILE_SIZE * 6.0f}, false};
    currentLevel.coins[1] = (Coin){{TILE_SIZE * 14.0f, WINDOW_HEIGHT - TILE_SIZE * 9.0f}, false};
    currentLevel.coins[2] = (Coin){{TILE_SIZE * 20.0f, WINDOW_HEIGHT - TILE_SIZE * 12.0f}, false};

    // Set flag position
    currentLevel.flagPosition = (Vector2){
        static_cast<float>(WINDOW_WIDTH - TILE_SIZE * 2),
        static_cast<float>(WINDOW_HEIGHT - TILE_SIZE * 2)
    };
}

void UpdateGame(float deltaTime) {
    if (IsKeyPressed(KEY_P)) {
        currentScreen = (currentScreen == GAME) ? PAUSE : GAME;
        return;
    }

    if (currentScreen == PAUSE) return;

    // Handle movement
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.velocity.x = MOVE_SPEED;
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.velocity.x = -MOVE_SPEED;
    } else {
        player.velocity.x = 0;
    }

    // Handle jumping
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && !player.isJumping) {
        player.velocity.y = -JUMP_FORCE;
        player.isJumping = true;
    }

    // Apply gravity
    player.velocity.y += GRAVITY * deltaTime;

    // Calculate new position
    Vector2 newPos = {
        player.position.x + player.velocity.x * deltaTime,
        player.position.y + player.velocity.y * deltaTime
    };

    // Create collision rectangles
    Rectangle playerRect = {
        newPos.x, newPos.y,
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    };

    // Check tile collisions
    bool collidingWithGround = false;
    int startTileX = static_cast<int>(newPos.x) / TILE_SIZE;
    int startTileY = static_cast<int>(newPos.y) / TILE_SIZE;
    int endTileX = static_cast<int>(newPos.x + TILE_SIZE) / TILE_SIZE;
    int endTileY = static_cast<int>(newPos.y + TILE_SIZE) / TILE_SIZE;

    for (int y = startTileY; y <= endTileY; y++) {
        for (int x = startTileX; x <= endTileX; x++) {
            if (x >= 0 && x < currentLevel.width && y >= 0 && y < currentLevel.height) {
                if (currentLevel.tiles[y * currentLevel.width + x]) {
                    Rectangle tileRect = {
                        static_cast<float>(x * TILE_SIZE),
                        static_cast<float>(y * TILE_SIZE),
                        static_cast<float>(TILE_SIZE),
                        static_cast<float>(TILE_SIZE)
                    };

                    if (CheckCollisionRecs(playerRect, tileRect)) {
                        if (player.velocity.y > 0) {
                            newPos.y = tileRect.y - TILE_SIZE;
                            collidingWithGround = true;
                            player.velocity.y = 0;
                        } else if (player.velocity.y < 0) {
                            newPos.y = tileRect.y + TILE_SIZE;
                            player.velocity.y = 0;
                        }
                    }
                }
            }
        }
    }

    // Update jumping state
    if (collidingWithGround) {
        player.isJumping = false;
    }

    // Keep player in bounds
    if (newPos.x < 0) newPos.x = 0;
    if (newPos.x > WINDOW_WIDTH - TILE_SIZE) newPos.x = WINDOW_WIDTH - TILE_SIZE;
    if (newPos.y < 0) newPos.y = 0;
    if (newPos.y > WINDOW_HEIGHT - TILE_SIZE) {
        newPos.y = WINDOW_HEIGHT - TILE_SIZE;
        player.isJumping = false;
        player.velocity.y = 0;
    }

    // Update player position
    player.position = newPos;

    // Check coin collisions
    for (int i = 0; i < currentLevel.coinCount; i++) {
        if (!currentLevel.coins[i].isCollected) {
            Rectangle coinRect = {
                currentLevel.coins[i].position.x,
                currentLevel.coins[i].position.y,
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            };

            if (CheckCollisionRecs(playerRect, coinRect)) {
                currentLevel.coins[i].isCollected = true;
                player.score += 100;
            }
        }
    }

    // Check victory condition
    Rectangle flagRect = {
        currentLevel.flagPosition.x,
        currentLevel.flagPosition.y,
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    };

    if (CheckCollisionRecs(playerRect, flagRect)) {
        currentScreen = VICTORY;
    }
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch(currentScreen) {
        case TITLE: {
            DrawText("LION'S HEART", 200, 100, 60, RED);
            DrawText("Start Game", 250, 200, 50,
                    (GetMouseY() >= 200 && GetMouseY() <= 250) ? RED : RAYWHITE);
            DrawText("Instructions", 250, 280, 50,
                    (GetMouseY() >= 280 && GetMouseY() <= 330) ? RED : RAYWHITE);
            DrawText("Quit", 250, 360, 50,
                    (GetMouseY() >= 360 && GetMouseY() <= 410) ? RED : RAYWHITE);
            break;
        }

        case GAME: {
            // Draw tiles
            for (int y = 0; y < currentLevel.height; y++) {
                for (int x = 0; x < currentLevel.width; x++) {
                    if (currentLevel.tiles[y * currentLevel.width + x]) {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE,
                                    TILE_SIZE, TILE_SIZE, BLUE);
                    }
                }
            }

            // Draw coins
            for (int i = 0; i < currentLevel.coinCount; i++) {
                if (!currentLevel.coins[i].isCollected) {
                    DrawRectangle(
                        static_cast<int>(currentLevel.coins[i].position.x),
                        static_cast<int>(currentLevel.coins[i].position.y),
                        TILE_SIZE, TILE_SIZE, YELLOW
                    );
                }
            }

            // Draw player
            DrawRectangle(
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                TILE_SIZE, TILE_SIZE, RED
            );

            // Draw flag
            DrawRectangle(
                static_cast<int>(currentLevel.flagPosition.x),
                static_cast<int>(currentLevel.flagPosition.y),
                TILE_SIZE, TILE_SIZE, GREEN
            );

            // Draw score
            DrawText(TextFormat("SCORE: %d", player.score),
                    WINDOW_WIDTH/2 - 100, 20, 40, WHITE);
            break;
        }

        case PAUSE: {
            DrawText("PAUSED", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 30,
                    60, WHITE);
            DrawText("Press P to continue", WINDOW_WIDTH/2 - 150,
                    WINDOW_HEIGHT/2 + 50, 20, WHITE);
            break;
        }

        case VICTORY: {
            DrawText("VICTORY!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 30,
                    60, GREEN);
            DrawText("Press SPACE to play again", WINDOW_WIDTH/2 - 150,
                    WINDOW_HEIGHT/2 + 50, 20, WHITE);
            break;
        }

        case INSTRUCTIONS: {
            DrawText("Instructions:", 200, 100, 30, RAYWHITE);
            DrawText("1. Use WASD or Arrow keys to move", 200, 150, 20, RAYWHITE);
            DrawText("2. Space or W to jump", 200, 200, 20, RAYWHITE);
            DrawText("3. Collect coins for points", 200, 250, 20, RAYWHITE);
            DrawText("4. P to pause game", 200, 300, 20, RAYWHITE);
            DrawText("Press SPACE to return to title", 200, 400, 20, RAYWHITE);
            break;
        }
    }

    EndDrawing();
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lion's Heart");
    SetTargetFPS(60);

    InitLevel();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Handle menu navigation
        if (currentScreen == TITLE) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int mouseY = GetMouseY();
                if (mouseY >= 200 && mouseY <= 250) {
                    currentScreen = GAME;
                } else if (mouseY >= 280 && mouseY <= 330) {
                    currentScreen = INSTRUCTIONS;
                } else if (mouseY >= 360 && mouseY <= 410) {
                    break;
                }
            }
        } else if (currentScreen == INSTRUCTIONS) {
            if (IsKeyPressed(KEY_SPACE)) {
                currentScreen = TITLE;
            }
        } else if (currentScreen == VICTORY) {
            if (IsKeyPressed(KEY_SPACE)) {
                InitLevel();
                currentScreen = GAME;
            }
        } else {
            UpdateGame(deltaTime);
        }

        DrawGame();
    }

    // Cleanup
    free(currentLevel.tiles);
    free(currentLevel.coins);
    CloseWindow();

    return 0;
}