#ifndef BASICBRICK_H
#define BASICBRICK_H

#include "bricks/Brick.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class BasicBrick : public Brick {
public:
    BasicBrick(float x, float y, int width, int height, int hitPoints);
    virtual ~BasicBrick() override;

    void render(SDL_Renderer* renderer) override;
    bool checkCollision(const SDL_Rect& ballRect) override;

private:
    int width, height;
    void calculateVertices();
};

#endif // BASICBRICK_H
