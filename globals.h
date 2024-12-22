#ifndef GLOBALS_H
#define GLOBALS_H

// Screen dimensions
const int screenWidth = 800;
const int screenHeight = 600;

// Game settings
const float GRAVITY = 0.05f;  // Gravity force (slowed down by 10x)
const float JUMP_STRENGTH = -1.0f;  //
const float PLAYER_SPEED = 0.5f;  // Player speed (slowed down by 10x)

// Player settings
struct Player {
    Rectangle rect;
    Vector2 speed;
    bool onGround;
};

// Level platforms
extern Rectangle platforms[];
extern int totalPlatforms;

#endif
