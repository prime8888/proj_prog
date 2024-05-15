#include "Utilities.h"
// #include <SDL_image.h>

namespace Utils {

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
        // Calculate the sides of rect A
        int leftA = a.x;
        int rightA = a.x + a.w;
        int topA = a.y;
        int bottomA = a.y + a.h;

        // Calculate the sides of rect B
        int leftB = b.x;
        int rightB = b.x + b.w;
        int topB = b.y;
        int bottomB = b.y + b.h;

        // Conditions for no collision
        if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
            return false;
        }

        return true; // Collision occurred
    }

    std::string determineCollisionSide(const SDL_Rect& ball, const SDL_Rect& brick) {
        float w = 0.5 * (ball.w + brick.w);
        float h = 0.5 * (ball.h + brick.h);
        float dx = (ball.x + ball.w / 2) - (brick.x + brick.w / 2);
        float dy = (ball.y + ball.h / 2) - (brick.y + brick.h / 2);

        if (abs(dx) <= w && abs(dy) <= h) {
            float wy = w * dy;
            float hx = h * dx;

            float overlapX = w - abs(dx);
            float overlapY = h - abs(dy);

            // Ajout d'un seuil pour aider à distinguer clairement les côtés
            if (overlapX > overlapY) {
                return (dy > 0) ? "BOTTOM" : "TOP";
            } else {
                return (dx > 0) ? "RIGHT" : "LEFT";
            }
        }
        return "NONE";
    }

    // SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    //     SDL_Texture* newTexture = nullptr;
    //     SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    //     if (loadedSurface == nullptr) {
    //         SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    //     } else {
    //         newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    //         if (newTexture == nullptr) {
    //             SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    //         }
    //         SDL_FreeSurface(loadedSurface);
    //     }
    //     return newTexture;
    // }

    SDL_Rect makeRect(int x, int y, int w, int h) {
        SDL_Rect rect = {x, y, w, h};
        return rect;
    }

    void handleBallCollision(Ball& ball, const SDL_Rect& obj) {
        SDL_Rect ballRect = makeRect(static_cast<int>(ball.getX()), static_cast<int>(ball.getY()), ball.getDiameter(), ball.getDiameter());
        if (checkCollision(ballRect, obj)) {
            // Reverse the ball's Y velocity to simulate a bounce
            ball.reverseY();
        }
    }

    // Helper function to project a point onto an axis
    float projectPointOntoAxis(const SDL_Point& point, const SDL_Point& axis) {
        float dotProduct = point.x * axis.x + point.y * axis.y;
        float axisLengthSquared = axis.x * axis.x + axis.y * axis.y;
        return dotProduct / std::sqrt(axisLengthSquared);
    }

    // Helper function to get min and max projections of a shape on an axis
    std::pair<float, float> getMinMaxProjection(const std::vector<SDL_Point>& vertices, const SDL_Point& axis) {
        float minProjection = projectPointOntoAxis(vertices[0], axis);
        float maxProjection = minProjection;
        for (const SDL_Point& vertex : vertices) {
            float projection = projectPointOntoAxis(vertex, axis);
            if (projection < minProjection) {
                minProjection = projection;
            }
            if (projection > maxProjection) {
                maxProjection = projection;
            }
        }
        return {minProjection, maxProjection};
    }

    // Helper function to reflect the ball's velocity
    SDL_Point reflectVelocity(const SDL_Point& velocity, const SDL_Point& normal) {
        float dotProduct = velocity.x * normal.x + velocity.y * normal.y;
        float normalLengthSquared = normal.x * normal.x + normal.y * normal.y;
        float scale = 2 * dotProduct / normalLengthSquared;
        SDL_Point reflection = {velocity.x - scale * normal.x, velocity.y - scale * normal.y};
        return reflection;
    }

}
