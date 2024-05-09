#pragma once

#include <SDL.h>

// Classe Ball pour gérer la logique de la balle dans le jeu Casse Brique
class Ball {
public:
    Ball(float x, float y, float velocityX, float velocityY, int diameter);
    ~Ball();

    // Fonctions pour gérer le mouvement et la mise à jour de la balle
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void resetCollisionTimer();
    bool canCollide() const;

    // Fonctions pour la gestion des collisions
    void reverseY();
    void reverseX();

    // Accesseurs
    float getX() const;
    float getY() const;
    int getDiameter() const;
    float getVelocityX() const;
    float getVelocityY() const;

    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);

private:
    float x, y;             // Position de la balle
    float velocityX, velocityY; // Vitesse de la balle sur l'axe X et Y
    int diameter;           // Diamètre de la balle
    float timeSinceLastCollision;
    const float COLLISION_COOLDOWN = 0.05;
};
