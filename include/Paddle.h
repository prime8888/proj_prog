#ifndef PADDLE_H
#define PADDLE_H

#include <SDL.h>
#include "Ball.h"

class Paddle {
public:
    Paddle(float x, float y, int width, int height, float speed);
    ~Paddle();

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void handleCollision(Ball& ball);

    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;
    void setPosition(float newX, float newY);
    void setWidth(int newWidth);

private:
    float x, y;
    int width, height;
    float speed;
    float velocity;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
};

#endif
