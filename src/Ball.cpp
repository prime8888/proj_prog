#include "Ball.h"
#include "iostream"

Ball::Ball(float x, float y, float velocityX, float velocityY, int diameter)
    : x(x), y(y), velocityX(velocityX), velocityY(velocityY), diameter(diameter) {}

Ball::~Ball() {}

void Ball::update(float deltaTime) {
    if (timeSinceLastCollision < COLLISION_COOLDOWN) {
        timeSinceLastCollision += deltaTime;
    }

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Boundary control to ensure the ball stays in the window
    if (x < 0 || x + diameter > 800) { // Make sure screen_width is set or accessible
        velocityX = -velocityX; // Reverses horizontal direction
        x = (x < 0) ? 0 : 800 - diameter;
    }
    if (y < 0 || y + diameter > 600) { // Make sure screen_height is set or accessible
        velocityY = -velocityY; // Reverses vertical direction
        y = (y < 0) ? 0 : 600 - diameter;
    }
}

void Ball::resetCollisionTimer() {
    timeSinceLastCollision = 0;
}

// Checks if the ball can undergo another collision
bool Ball::canCollide() const {
    return timeSinceLastCollision >= COLLISION_COOLDOWN;
}

void Ball::render(SDL_Renderer* renderer) {
    // Defines the color of the ball (here green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Draw the ball as a solid circle
    filledCircleColor(renderer, static_cast<int>(x), static_cast<int>(y), diameter/2, 0xFF90FB3B);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Ball::reverseY() {
    velocityY = -velocityY;
}

void Ball::reverseX() {
    velocityX = -velocityX;
}

float Ball::getX() const {
    return x;
}

float Ball::getY() const {
    return y;
}

float Ball::getVelocityX() const {
    return velocityX;
}

float Ball::getVelocityY() const {
    return velocityY;
}

int Ball::getDiameter() const {
    return diameter;
}

void Ball::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Ball::setVelocity(float newVx, float newVy) {
    velocityX = newVx;
    velocityY = newVy;
}
