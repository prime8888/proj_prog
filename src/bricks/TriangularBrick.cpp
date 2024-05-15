#include "bricks/TriangularBrick.h"

TriangularBrick::TriangularBrick(float x, float y, float base, float height, bool inverted, int hitPoints)
    : Brick(x, y, hitPoints), base(base), height(height), inverted(inverted) {
    calculateVertices();
}

TriangularBrick::~TriangularBrick() {}

void TriangularBrick::calculateVertices() {
    vertices.clear();
    if (inverted) {
        // Inverted triangle (base at the top)
        vertices.push_back({static_cast<int>(x), static_cast<int>(y)});
        vertices.push_back({static_cast<int>(x + base / 2), static_cast<int>(y + height)});
        vertices.push_back({static_cast<int>(x + base), static_cast<int>(y)});
    } else {
        // Upright triangle (base at the bottom)
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
        filledPolygonColor(renderer, vx, vy, vertices.size(), 0xFF0000FF);
        // Draw hexagon border with black color
        polygonColor(renderer, vx, vy, vertices.size(), 0xFF000000);
    }
}

bool TriangularBrick::checkCollision(const SDL_Rect& ballRect) {
    std::vector<SDL_Point> ballVertices = {
        {ballRect.x, ballRect.y},
        {ballRect.x + ballRect.w, ballRect.y},
        {ballRect.x + ballRect.w, ballRect.y + ballRect.h},
        {ballRect.x, ballRect.y + ballRect.h}
    };

    std::vector<SDL_Point> axes;
    // Edges of the triangle
    for (size_t i = 0; i < vertices.size(); ++i) {
        SDL_Point p1 = vertices[i];
        SDL_Point p2 = vertices[(i + 1) % vertices.size()];
        SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
        SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
        axes.push_back(normal);
    }

    // Edges of the ball's bounding box
    for (size_t i = 0; i < ballVertices.size(); ++i) {
        SDL_Point p1 = ballVertices[i];
        SDL_Point p2 = ballVertices[(i + 1) % ballVertices.size()];
        SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
        SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
        axes.push_back(normal);
    }

    // Perform SAT for all axes
    for (const SDL_Point& axis : axes) {
        auto [minTriangle, maxTriangle] = Utils::getMinMaxProjection(vertices, axis);
        auto [minBall, maxBall] = Utils::getMinMaxProjection(ballVertices, axis);

        if (maxTriangle < minBall || maxBall < minTriangle) {
            return false; // No collision
        }
    }

    return true; // Collision occurred
}

std::string TriangularBrick::determineCollisionSide(const SDL_Rect& ball) {
    // Implement collision side determination
    return "NONE";
}

void TriangularBrick::handleCollision(Ball& ball) {
    SDL_Rect ballRect = {static_cast<int>(ball.getX()), static_cast<int>(ball.getY()), ball.getDiameter(), ball.getDiameter()};
    if (!this->isDestroyed() && this->checkCollision(ballRect) && ball.canCollide()) {
        ball.resetCollisionTimer();
        this->hit();

        // Calculate the normal for each edge of the triangle
        std::vector<SDL_Point> normals;
        for (size_t i = 0; i < vertices.size(); ++i) {
            SDL_Point p1 = vertices[i];
            SDL_Point p2 = vertices[(i + 1) % vertices.size()];
            SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
            SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
            normals.push_back(normal);
        }

        // Determine which edge the ball collided with by checking the minimum penetration depth
        SDL_Point ballCenter = {ballRect.x + ballRect.w / 2, ballRect.y + ballRect.h / 2};
        float minPenetration = std::numeric_limits<float>::max();
        SDL_Point collisionNormal = normals[0];

        for (const SDL_Point& normal : normals) {
            float distance = std::abs((ballCenter.x - vertices[0].x) * normal.x + (ballCenter.y - vertices[0].y) * normal.y) /
                             std::sqrt(normal.x * normal.x + normal.y * normal.y);

            if (distance < minPenetration) {
                minPenetration = distance;
                collisionNormal = normal;
            }
        }

        // Reflect the ball's velocity based on the collision normal
        SDL_Point ballVelocity = {static_cast<int>(ball.getVelocityX()), static_cast<int>(ball.getVelocityY())};
        SDL_Point newVelocity = Utils::reflectVelocity(ballVelocity, collisionNormal);

        ball.setVelocity(newVelocity.x, newVelocity.y);
    }
}

float TriangularBrick::getBase() const {
    return base;
}

float TriangularBrick::getHeight() const {
    return height;
}