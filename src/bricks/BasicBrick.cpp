#include "bricks/BasicBrick.h"

BasicBrick::BasicBrick(float x, float y, int width, int height, int hitPoints)
    : x(x), y(y), width(width), height(height), hitPoints(hitPoints), destroyed(false) {}

BasicBrick::~BasicBrick() {}

void BasicBrick::hit() {
    hitPoints--;
    if (hitPoints <= 0) {
        destroyed = true;
    }
}

bool BasicBrick::isDestroyed() const {
    return destroyed;
}

void BasicBrick::render(SDL_Renderer* renderer) {
    // Set brick color (e.g., red if not destroyed)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color

    // Only render if not destroyed
    if (!destroyed) {
        SDL_Rect brickRect = {static_cast<int>(x), static_cast<int>(y), width, height};
        SDL_RenderFillRect(renderer, &brickRect);
    }
}

float BasicBrick::getX() const {
    return x;
}

float BasicBrick::getY() const {
    return y;
}

int BasicBrick::getWidth() const {
    return width;
}

int BasicBrick::getHeight() const {
    return height;
}

void BasicBrick::reset() {
    destroyed = false;
    // Reset hitPoints to original value or another appropriate value
    hitPoints = 1; // Assuming initial hitPoints should be reset to 1
}
