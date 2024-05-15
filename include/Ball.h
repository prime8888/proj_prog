#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

// Ball class to manage the logic of the ball in the Brick Break game
class Ball {
public:
    Ball(float x, float y, float velocityX, float velocityY, int diameter);
    ~Ball();

    // Functions to manage the movement and updating of the ball
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void resetCollisionTimer();
    bool canCollide() const;

    // Functions for collision management
    void reverseY();
    void reverseX();

    // Accessors
    float getX() const;
    float getY() const;
    int getDiameter() const;
    float getVelocityX() const;
    float getVelocityY() const;

    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);

private:
    float x, y;             // Ball position
    float velocityX, velocityY; // Ball speed on the X and Y axis
    int diameter;           // Ball diameter
    float timeSinceLastCollision;
    const float COLLISION_COOLDOWN = 0.05;
};
