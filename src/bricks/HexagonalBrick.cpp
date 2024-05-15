#include "bricks/HexagonalBrick.h"
#include <cmath>

HexagonalBrick::HexagonalBrick(float x, float y, float sideLength, int hitPoints)
    : Brick(x, y, hitPoints), sideLength(sideLength) {
    calculateVertices();
}

HexagonalBrick::~HexagonalBrick() {}

void HexagonalBrick::calculateVertices() {
    vertices.clear();
    for (int i = 0; i < 6; ++i) {
        float angle = M_PI / 3.0 * i;
        int vx = static_cast<int>(x + sideLength * cos(angle));
        int vy = static_cast<int>(y + sideLength * sin(angle));
        vertices.push_back({vx, vy});
    }
}

void HexagonalBrick::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        Sint16 vx[6], vy[6];
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

        // Draw hexagon border with black color
        polygonColor(renderer, vx, vy, vertices.size(), 0xFF000000);
    }
}
