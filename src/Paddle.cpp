#include "Paddle.h"
#include <SDL.h>

Paddle::Paddle(float x, float y, int width, int height, float speed)
    : x(x), y(y), width(width), height(height), speed(speed), velocity(0) {}

Paddle::~Paddle() {}

void Paddle::moveLeft(float deltaTime) {
    // Update the velocity to move left
    velocity = -speed;
    x += velocity * deltaTime;
    // Ensure the paddle doesn't move out of the screen bounds
    if (x < 0) {
        x = 0;
    }
}

void Paddle::moveRight(float deltaTime) {
    // Update the velocity to move right
    velocity = speed;
    x += velocity * deltaTime;
    // Check boundaries
    if (x + width > 800) { // Assuming screen width is 800
        x = 800 - width;
    }
}

void Paddle::update(float deltaTime) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT]) {
        moveLeft(deltaTime);
    } else if (keystate[SDL_SCANCODE_RIGHT]) {
        moveRight(deltaTime);
    } else {
        velocity = 0; // Stop the paddle when no keys are pressed
    }
    // Update the paddle's position based on current velocity
    x += velocity * deltaTime;
}

void Paddle::render(SDL_Renderer* renderer) {
    // Set paddle color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color

    // Draw paddle as a rectangle
    SDL_Rect paddleRect = {static_cast<int>(x), static_cast<int>(y), width, height};
    SDL_RenderFillRect(renderer, &paddleRect);
}

float Paddle::getX() const {
    return x;
}

float Paddle::getY() const {
    return y;
}

int Paddle::getWidth() const {
    return width;
}

int Paddle::getHeight() const {
    return height;
}

void Paddle::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}