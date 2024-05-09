#pragma once

#include <SDL.h>

// Paddle class to manage the paddle logic in the Breakout game
class Paddle {
public:
    Paddle(float x, float y, int width, int height, float speed);
    ~Paddle();

    // Functions to manage paddle movement
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Accessors
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;

    void setPosition(float x, float y);

private:
    float x, y;     // Position of the paddle
    int width, height; // Size of the paddle
    float speed;    // Speed of the paddle's movement
    float velocity; // Current velocity of the paddle
};
