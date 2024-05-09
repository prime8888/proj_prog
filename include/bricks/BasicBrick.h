#pragma once

#include <SDL.h>

// BasicBrick class to manage the logic of bricks in the Breakout game
class BasicBrick {
public:
    BasicBrick(float x, float y, int width, int height, int hitPoints);
    ~BasicBrick();

    // Functions to manage the brick's state
    void hit(); // Called when the brick is hit by the ball
    bool isDestroyed() const; // Check if the brick is destroyed
    void render(SDL_Renderer* renderer); // Render the brick
    void reset();

    // Accessors
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;
    

private:
    float x, y;           // Position of the brick
    int width, height;    // Size of the brick
    int hitPoints;        // Number of hits the brick can take before being destroyed
    bool destroyed;       // Status of the brick
};
