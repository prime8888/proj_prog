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

}
