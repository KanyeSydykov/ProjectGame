#ifndef GLOBALS_H
#define GLOBALS_H

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
    INSTRUCTIONS,
    QUIT
};

// Game assets
struct GameTextures {
    Texture2D playerTexture;
    Texture2D brickTexture;
    Texture2D coinTexture;
    Texture2D flagTexture;
};

struct GameSounds {
    Sound jumpSound;
    Sound coinSound;
    Sound victorySound;
};

struct GameAssets {
    GameTextures textures;
    GameSounds sounds;
    Font gameFont;
};

// Player structure
struct Player {
    Vector2 position;
    Vector2 velocity;
    bool isJumping;
    int score;
};

// Coin structure
struct Coin {
    Vector2 position;
    bool isCollected;
};

// Level structure
struct Level {
    bool* tiles;  // true for solid tiles
    int width;
    int height;
    Vector2 flagPosition;
    Coin* coins;
    int coinCount;
};

// Global variables
extern Player player;
extern Level currentLevel;
extern GameAssets assets;
extern GameScreen currentScreen;

// Function declarations
void LoadGameAssets();
void UnloadGameAssets();
void InitLevel();

#endif // GLOBALS_H