#pragma once

#include "bricks/Brick.h"
#include <string>

class BasicBrick : public Brick {
public:
    BasicBrick(float x, float y, int width, int height, int hitPoints);
    virtual ~BasicBrick() override;
    void render(SDL_Renderer* renderer) override;
    void handleCollision(Ball& ball) override;

    int getWidth() const;
    int getHeight() const;

private:
    int width, height;

    std::string determineCollisionSide(const SDL_Rect& ball);
    bool checkCollision(const SDL_Rect& ballRect) override;
};
