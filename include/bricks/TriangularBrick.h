#ifndef TRIANGULARBRICK_H
#define TRIANGULARBRICK_H

#include "Brick.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string>
#include "Utilities.h"

class TriangularBrick : public Brick {
public:
    TriangularBrick(float x, float y, float base, float height, bool inverted, int hitPoints);
    virtual ~TriangularBrick() override;

    void render(SDL_Renderer* renderer) override;
    void handleCollision(Ball& ball) override;

    float getBase() const;
    float getHeight() const;

private:
    float base, height;
    std::vector<SDL_Point> vertices;
    bool inverted;

    void calculateVertices();
    std::string determineCollisionSide(const SDL_Rect& ball);
    bool checkCollision(const SDL_Rect& ballRect) override;
};

#endif // TRIANGULARBRICK_H