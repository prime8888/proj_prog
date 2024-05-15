#ifndef TRIANGULARBRICK_H
#define TRIANGULARBRICK_H

#include "bricks/Brick.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class TriangularBrick : public Brick {
public:
    TriangularBrick(float x, float y, float base, float height, bool inverted, int hitPoints);
    virtual ~TriangularBrick() override;

    void render(SDL_Renderer* renderer) override;

private:
    float base, height;
    bool inverted;
    void calculateVertices();
};

#endif // TRIANGULARBRICK_H
