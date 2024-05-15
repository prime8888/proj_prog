#ifndef HEXAGONALBRICK_H
#define HEXAGONALBRICK_H

#include "bricks/Brick.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class HexagonalBrick : public Brick {
public:
    HexagonalBrick(float x, float y, float sideLength, int hitPoints);
    virtual ~HexagonalBrick() override;

    void render(SDL_Renderer* renderer) override;

private:
    float sideLength;
    void calculateVertices();
};

#endif // HEXAGONALBRICK_H
