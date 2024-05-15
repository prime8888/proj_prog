#include "Ball.h"
#include "iostream"

Ball::Ball(float x, float y, float velocityX, float velocityY, int diameter)
    : x(x), y(y), velocityX(velocityX), velocityY(velocityY), diameter(diameter) {}

Ball::~Ball() {}

void Ball::update(float deltaTime) {
    if (timeSinceLastCollision < COLLISION_COOLDOWN) {
        timeSinceLastCollision += deltaTime;
    }

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Contrôle des limites pour s'assurer que la balle reste dans la fenêtre
    if (x < 0 || x + diameter > 800) { // Assurez-vous que screen_width est défini ou accessible
        velocityX = -velocityX; // Inverse la direction horizontale
        x = (x < 0) ? 0 : 800 - diameter;
    }
    if (y < 0 || y + diameter > 600) { // Assurez-vous que screen_height est défini ou accessible
        velocityY = -velocityY; // Inverse la direction verticale
        y = (y < 0) ? 0 : 600 - diameter;
    }

    // std::cout << "Ball position: " << x << ", " << y << std::endl;
    // std::cout << "Ball velocity: " << velocityX << ", " << velocityY << std::endl;
}

void Ball::resetCollisionTimer() {
    timeSinceLastCollision = 0;
}

// Vérifier si la balle peut subir une autre collision
bool Ball::canCollide() const {
    return timeSinceLastCollision >= COLLISION_COOLDOWN;
}

void Ball::render(SDL_Renderer* renderer) {
    // Définit la couleur de la balle (ici vert)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Dessine la balle comme un cercle plein
    filledCircleColor(renderer, static_cast<int>(x), static_cast<int>(y), diameter/2, 0xFF90FB3B);

    // Utilisez une fonction appropriée pour dessiner un cercle avec SDL2
    // Par exemple, en utilisant SDL_gfx ou une autre bibliothèque graphique
    //SDL_Rect ballRect = {static_cast<int>(x), static_cast<int>(y), diameter, diameter};
    //SDL_RenderFillRect(renderer, &ballRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Ball::reverseY() {
    velocityY = -velocityY;
}

void Ball::reverseX() {
    velocityX = -velocityX;
}

float Ball::getX() const {
    return x;
}

float Ball::getY() const {
    return y;
}

float Ball::getVelocityX() const {
    return velocityX;
}

float Ball::getVelocityY() const {
    return velocityY;
}

int Ball::getDiameter() const {
    return diameter;
}

void Ball::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Ball::setVelocity(float newVx, float newVy) {
    velocityX = newVx;
    velocityY = newVy;
}
