#include "bricks/Brick.h"

Brick::Brick(float x, float y, int hitPoints)
    : x(x), y(y), hitPoints(hitPoints), destroyed(false) {}

Brick::~Brick() {}

void Brick::hit() {
    hitPoints--;
    if (hitPoints <= 0) {
        destroyed = true;
    }
}

bool Brick::isDestroyed() const {
    return destroyed;
}

float Brick::getX() const {
    return x;
}

float Brick::getY() const {
    return y;
}

void Brick::reset() {
    destroyed = false;
    hitPoints = 1;
}