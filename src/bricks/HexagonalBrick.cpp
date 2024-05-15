#include "bricks/HexagonalBrick.h"
#include <cmath>

HexagonalBrick::HexagonalBrick(float x, float y, float sideLength, int hitPoints)
    : Brick(x, y, hitPoints), sideLength(sideLength) {
    calculateVertices();
}

HexagonalBrick::~HexagonalBrick() {}

void HexagonalBrick::calculateVertices() {
    vertices.clear();
    float angle = 0.0f;
    for (int i = 0; i < 6; ++i) {
        int vx = static_cast<int>(x + sideLength * std::cos(angle));
        int vy = static_cast<int>(y + sideLength * std::sin(angle));
        vertices.push_back({vx, vy});
        angle += M_PI / 3.0f;
    }
}

void HexagonalBrick::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        Sint16 vx[6], vy[6];
        for (size_t i = 0; i < vertices.size(); ++i) {
            vx[i] = vertices[i].x;
            vy[i] = vertices[i].y;
        }
        filledPolygonColor(renderer, vx, vy, vertices.size(), 0xFF00FF00); // Green color

        // Draw hexagon border with black color
        polygonColor(renderer, vx, vy, vertices.size(), 0xFF000000);
    }
}

bool HexagonalBrick::checkCollision(const SDL_Rect& ballRect) {
    // Define the ball vertices (bounding box)
    SDL_Point ballVertices[4] = {
        {ballRect.x, ballRect.y},
        {ballRect.x + ballRect.w, ballRect.y},
        {ballRect.x + ballRect.w, ballRect.y + ballRect.h},
        {ballRect.x, ballRect.y + ballRect.h}
    };

    // Define axes to test (normals of hexagon edges)
    std::vector<SDL_Point> axes;
    for (size_t i = 0; i < vertices.size(); ++i) {
        SDL_Point p1 = vertices[i];
        SDL_Point p2 = vertices[(i + 1) % vertices.size()];
        SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
        SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
        axes.push_back(normal);
    }

    // Add axes of the ball (normals of bounding box edges)
    for (size_t i = 0; i < 4; ++i) {
        SDL_Point p1 = ballVertices[i];
        SDL_Point p2 = ballVertices[(i + 1) % 4];
        SDL_Point edge = {p2.x - p1.x, p2.y - p1.y};
        SDL_Point normal = {-edge.y, edge.x}; // Perpendicular vector (normal)
        axes.push_back(normal);
    }

    // Function to project a point onto an axis
    auto project = [](const SDL_Point& point, const SDL_Point& axis) -> float {
        float dotProduct = point.x * axis.x + point.y * axis.y;
        float axisLengthSquared = axis.x * axis.x + axis.y * axis.y;
        return dotProduct / std::sqrt(axisLengthSquared);
    };

    // Function to get min and max projection of a shape on an axis
    auto getMinMaxProjection = [&project](const std::vector<SDL_Point>& vertices, const SDL_Point& axis) -> std::pair<float, float> {
        float minProjection = project(vertices[0], axis);
        float maxProjection = minProjection;
        for (const SDL_Point& vertex : vertices) {
            float projection = project(vertex, axis);
            if (projection < minProjection) {
                minProjection = projection;
            }
            if (projection > maxProjection) {
                maxProjection = projection;
            }
        }
        return {minProjection, maxProjection};
    };

    // Perform SAT
    for (const SDL_Point& axis : axes) {
        auto [minHex, maxHex] = getMinMaxProjection(vertices, axis);
        std::vector<SDL_Point> ballVerticesVec(ballVertices, ballVertices + 4);
        auto [minBall, maxBall] = getMinMaxProjection(ballVerticesVec, axis);

        if (maxHex < minBall || maxBall < minHex) {
            return false; // No collision
        }
    }

    return true; // Collision occurred
}

std::string HexagonalBrick::determineCollisionSide(const SDL_Rect& ball) {
    SDL_Point ballCenter = {ball.x + ball.w / 2, ball.y + ball.h / 2};
    SDL_Point brickCenter = {static_cast<int>(x), static_cast<int>(y)};
    
    float minDistance = std::numeric_limits<float>::max();
    std::string closestSide = "NONE";
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        SDL_Point p1 = vertices[i];
        SDL_Point p2 = vertices[(i + 1) % vertices.size()];
        
        // Calculate edge midpoint
        SDL_Point edgeMidPoint = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
        
        // Calculate the vector from ball center to edge midpoint
        float dx = edgeMidPoint.x - ballCenter.x;
        float dy = edgeMidPoint.y - ballCenter.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < minDistance) {
            minDistance = distance;
            if (i == 0) {
                closestSide = "TOP-RIGHT";
            } else if (i == 1) {
                closestSide = "RIGHT";
            } else if (i == 2) {
                closestSide = "BOTTOM-RIGHT";
            } else if (i == 3) {
                closestSide = "BOTTOM-LEFT";
            } else if (i == 4) {
                closestSide = "LEFT";
            } else if (i == 5) {
                closestSide = "TOP-LEFT";
            }
        }
    }
    
    return closestSide;
}

void HexagonalBrick::handleCollision(Ball& ball) {
    SDL_Rect ballRect = {static_cast<int>(ball.getX()), static_cast<int>(ball.getY()), ball.getDiameter(), ball.getDiameter()};
    if (!this->isDestroyed() && this->checkCollision(ballRect) && ball.canCollide()) {
        ball.resetCollisionTimer();
        this->hit();

        // Calculate the normal for each edge of the hexagon
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

float HexagonalBrick::getSideLength() const {
    return sideLength;
}