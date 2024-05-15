#include "bricks/TriangularBrick.h"

TriangularBrick::TriangularBrick(float x, float y, float base, float height, bool inverted, int hitPoints)
    : Brick(x, y, hitPoints), base(base), height(height), inverted(inverted) {
    calculateVertices();
}

TriangularBrick::~TriangularBrick() {}

void TriangularBrick::calculateVertices() {
    vertices.clear();
    if (inverted) {
        vertices.push_back({static_cast<int>(x), static_cast<int>(y)});
        vertices.push_back({static_cast<int>(x + base / 2), static_cast<int>(y + height)});
        vertices.push_back({static_cast<int>(x + base), static_cast<int>(y)});
    } else {
        vertices.push_back({static_cast<int>(x), static_cast<int>(y + height)});
        vertices.push_back({static_cast<int>(x + base / 2), static_cast<int>(y)});
        vertices.push_back({static_cast<int>(x + base), static_cast<int>(y + height)});
    }
}

void TriangularBrick::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        Sint16 vx[3], vy[3];
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

        // Draw triangle border with black color
        polygonColor(renderer, vx, vy, vertices.size(), 0xFF000000);
    }
}
