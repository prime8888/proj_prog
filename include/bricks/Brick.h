#ifndef BRICK_H
#define BRICK_H

#include "SDL.h"
#include "Ball.h"
#include <vector>
#include <random>
#include <SDL2/SDL2_gfxPrimitives.h>

class Brick {
public:
    Brick(float x, float y, int hitPoints);
    virtual ~Brick() {}
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void handleCollision(Ball& ball, bool& addBalls, bool& expandPaddle);
    virtual bool checkCollision(const SDL_Rect& ballRect); // Moved to base class
    void hit(); // Called when the brick is hit by the ball
    bool isDestroyed() const; // Check if the brick is destroyed

    void setHitPoints(int hp);

    float getX() const;
    float getY() const;

    void reset();

protected:
    float x, y;
    int hitPoints;        // Number of hits the brick can take before being destroyed
    bool destroyed;       // Status of the brick
    std::vector<SDL_Point> vertices;

};

#endif
