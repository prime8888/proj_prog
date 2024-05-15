#ifndef HEXAGONALBRICK_H
#define HEXAGONALBRICK_H

#include "Brick.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string>
#include "Utilities.h"

class HexagonalBrick : public Brick {
public:
    HexagonalBrick(float x, float y, float sideLength, int hitPoints);
    virtual ~HexagonalBrick() override;

    void render(SDL_Renderer* renderer) override;
    void handleCollision(Ball& ball) override;

    float getSideLength() const;

private:
    float sideLength;
    std::vector<SDL_Point> vertices;

    void calculateVertices();
    std::string determineCollisionSide(const SDL_Rect& ball);
    bool checkCollision(const SDL_Rect& ballRect) override;
};

#endif // HEXAGONALBRICK_H