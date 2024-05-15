#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include "Ball.h"
#include "Paddle.h"
#include "bricks/BasicBrick.h"

// Namespace to contain utility functions
namespace Utils {

    // Checks for AABB (Axis-Aligned Bounding Box) collision
    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);

    // Load a texture from a file
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);

    // Helper function to create an SDL_Rect
    SDL_Rect makeRect(int x, int y, int w, int h);

    // Checks collision between ball and paddle or bricks
    void handleBallCollision(Ball& ball, const SDL_Rect& obj);

    std::string determineCollisionSide(const SDL_Rect& ball, const SDL_Rect& brick);

    float projectPointOntoAxis(const SDL_Point& point, const SDL_Point& axis);

    std::pair<float, float> getMinMaxProjection(const std::vector<SDL_Point>& vertices, const SDL_Point& axis);

    SDL_Point reflectVelocity(const SDL_Point& velocity, const SDL_Point& normal);
}
