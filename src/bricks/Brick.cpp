#include "bricks/Brick.h"
#include <cmath>
#include <limits>

// Helper function to reflect the ball's velocity
SDL_Point reflectVelocity(const SDL_Point& velocity, const SDL_Point& normal) {
    float dotProduct = velocity.x * normal.x + velocity.y * normal.y;
    float scale = 2 * dotProduct / (normal.x * normal.x + normal.y * normal.y);
    SDL_Point reflection = {velocity.x - scale * normal.x, velocity.y - scale * normal.y};
    return reflection;
}

Brick::Brick(float x, float y, int hitPoints)
    : x(x), y(y), hitPoints(hitPoints), destroyed(false) {}

void Brick::hit() {
    hitPoints--;
    if (hitPoints <= 0) {
        destroyed = true;
    }
}

bool Brick::isDestroyed() const {
    return destroyed;
}

float Brick::getX() const {
    return x;
}

float Brick::getY() const {
    return y;
}

void Brick::reset() {
    destroyed = false;
    hitPoints = 1;
}

void Brick::setHitPoints(int hp) {
    hitPoints = hp;
}

bool Brick::checkCollision(const SDL_Rect& ballRect) {
    // Axis-Aligned Bounding Box for simplicity
    SDL_Point ballCenter = {ballRect.x + ballRect.w / 2, ballRect.y + ballRect.h / 2};

    // Separating Axis Theorem (SAT) for convex polygon collision detection
    std::vector<SDL_Point> axes;
    for (size_t i = 0; i < vertices.size(); ++i) {
        SDL_Point p1 = vertices[i];
        SDL_Point p2 = vertices[(i + 1) % vertices.size()];
        SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
        SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
        axes.push_back(normal);
    }

    for (const auto& axis : axes) {
        // Project ball and polygon vertices onto axis
        float minPolygon = std::numeric_limits<float>::max();
        float maxPolygon = std::numeric_limits<float>::lowest();
        for (const auto& vertex : vertices) {
            float projection = (vertex.x * axis.x + vertex.y * axis.y) /
                               std::sqrt(axis.x * axis.x + axis.y * axis.y);
            minPolygon = std::min(minPolygon, projection);
            maxPolygon = std::max(maxPolygon, projection);
        }

        // Project ball center onto axis
        float ballProjection = (ballCenter.x * axis.x + ballCenter.y * axis.y) /
                               std::sqrt(axis.x * axis.x + axis.y * axis.y);
        float minBall = ballProjection - ballRect.w / 2;
        float maxBall = ballProjection + ballRect.w / 2;

        // Check for overlap
        if (maxPolygon < minBall || maxBall < minPolygon) {
            return false; // No collision
        }
    }

    return true; // Collision occurred
}

void Brick::handleCollision(Ball& ball) {
    SDL_Rect ballRect = {static_cast<int>(ball.getX()), static_cast<int>(ball.getY()), ball.getDiameter(), ball.getDiameter()};
    if (!this->isDestroyed() && this->checkCollision(ballRect) && ball.canCollide()) {
        ball.resetCollisionTimer();
        this->hit();

        // Calculate the normal for each edge of the brick
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
        SDL_Point newVelocity = reflectVelocity(ballVelocity, collisionNormal);

        ball.setVelocity(newVelocity.x, newVelocity.y);
    }
}
