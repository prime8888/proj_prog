#pragma once

#include <SDL.h>
#include "Paddle.h"
#include "Ball.h"

// Abstract base class for all bonuses
class Bonus {
public:
    Bonus(int x, int y, int w, int h, SDL_Texture* texture);
    virtual ~Bonus() = default;

    virtual void activate(Paddle& paddle, Ball& ball) = 0; // Activate the bonus effect
    void render(SDL_Renderer* renderer);
    bool isCollected() const { return collected; }
    void collect() { collected = true; }

protected:
    SDL_Rect rect;
    SDL_Texture* texture;
    bool collected;

    void moveDown(float speed); // Move the bonus downwards
};

// MultiBall bonus: creates additional balls in the game
class MultiBall : public Bonus {
public:
    MultiBall(int x, int y, SDL_Texture* texture);
    void activate(Paddle& paddle, Ball& ball) override;
};

// Other bonus types can be defined here
// For example: ExpandPaddle, SpeedUp, ExtraLife, etc.
