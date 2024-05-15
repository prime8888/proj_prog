#include "bricks/BasicBrick.h"

BasicBrick::BasicBrick(float x, float y, int width, int height, int hitPoints)
    : Brick(x, y, hitPoints), width(width), height(height) {}

BasicBrick::~BasicBrick() {}

void BasicBrick::render(SDL_Renderer* renderer) {
    // Set brick color (e.g., red if not destroyed)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color

    // Only render if not destroyed
    if (!destroyed) {
        SDL_Rect brickRect = {static_cast<int>(x), static_cast<int>(y), width, height};
        SDL_RenderFillRect(renderer, &brickRect);
    }
}

bool BasicBrick::checkCollision(const SDL_Rect& ballRect) {
    SDL_Rect a = {static_cast<int>(x), static_cast<int>(y), width, height};
    // Calculate the sides of rect A
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    // Calculate the sides of rect B
    int leftB = ballRect.x;
    int rightB = ballRect.x + ballRect.w;
    int topB = ballRect.y;
    int bottomB = ballRect.y + ballRect.h;

    // Conditions for no collision
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
        return false;
    }

    return true; // Collision occurred

}

std::string BasicBrick::determineCollisionSide(const SDL_Rect& ball) {
    SDL_Rect brick = {static_cast<int>(x), static_cast<int>(y), width, height};
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

void BasicBrick::handleCollision(Ball& ball) {
    SDL_Rect ballRect = {static_cast<int>(ball.getX()), static_cast<int>(ball.getY()), ball.getDiameter(), ball.getDiameter()};
    if (!this->isDestroyed()) {
        if (this->checkCollision(ballRect) && ball.canCollide()) {
            ball.resetCollisionTimer();
            std::string collisionSide = this->determineCollisionSide(ballRect);
            this->hit(); // Process the hit

            // Calculer le point d'impact
            float impactPointX = (ball.getX() + ball.getDiameter() / 2) - (x + width / 2);
            float normalizedImpact = impactPointX / (width / 2); // Normalisé entre -1 et 1

            // Ajuster la vitesse basée sur le point d'impact
            const float MAX_ANGLE = M_PI / 3.0; // Angle maximum de déviation en radians
            float angleAdjustment = normalizedImpact * MAX_ANGLE;
            

            if (collisionSide == "TOP" || collisionSide == "BOTTOM") {
                ball.setVelocity(ball.getVelocityX() + angleAdjustment, -ball.getVelocityY());
            } else if (collisionSide == "LEFT" || collisionSide == "RIGHT") {
                ball.setVelocity(-ball.getVelocityX(), ball.getVelocityY() + angleAdjustment);
            }
        }
    }
}
