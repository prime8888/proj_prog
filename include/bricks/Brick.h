#ifndef BRICK_H
#define BRICK_H

#include "SDL.h"
#include "Ball.h"

class Brick {
public:
    Brick(float x, float y, int hitPoints);
    virtual ~Brick();
    
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void handleCollision(Ball& ball) = 0;
    void hit(); // Called when the brick is hit by the ball
    bool isDestroyed() const; // Check if the brick is destroyed

    float getX() const;
    float getY() const;

    void reset();

protected:
    float x, y;
    int hitPoints;        // Number of hits the brick can take before being destroyed
    bool destroyed;       // Status of the brick

    virtual bool checkCollision(const SDL_Rect& ballRect) = 0;
};

#endif