#include "bricks/BasicBrick.h"

BasicBrick::BasicBrick(float x, float y, int width, int height, int hitPoints)
    : Brick(x, y, hitPoints), width(width), height(height) {
    calculateVertices();
}

BasicBrick::~BasicBrick() {}

void BasicBrick::calculateVertices() {
    vertices.clear();
    vertices.push_back({static_cast<int>(x), static_cast<int>(y)});
    vertices.push_back({static_cast<int>(x + width), static_cast<int>(y)});
    vertices.push_back({static_cast<int>(x + width), static_cast<int>(y + height)});
    vertices.push_back({static_cast<int>(x), static_cast<int>(y + height)});
}

void BasicBrick::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        Sint16 vx[4], vy[4];
        for (size_t i = 0; i < vertices.size(); ++i) {
            vx[i] = vertices[i].x;
            vy[i] = vertices[i].y;
        }

        Uint32 color;
        if (hitPoints == 1) {
            color = 0xFF0000FF; // Blue
        } else if (hitPoints == 2) {
            color = 0xFFFFFF00; // Yellow
        } else if (hitPoints >= 3) {
            color = 0xFFFF0000; // Red
        }

        filledPolygonColor(renderer, vx, vy, vertices.size(), color);

        // Draw brick border with black color
        polygonColor(renderer, vx, vy, vertices.size(), 0xFF000000);
    }
}

bool BasicBrick::checkCollision(const SDL_Rect& ballRect) {
    // Use axis-aligned bounding box collision detection for simplicity
    SDL_Rect a = {static_cast<int>(x), static_cast<int>(y), width, height};
    return SDL_HasIntersection(&a, &ballRect);
}
