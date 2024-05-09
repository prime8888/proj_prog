#include "Level.h"
#include "Utilities.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Level::Level(SDL_Renderer* renderer)
    : renderer(renderer), screen_width(800), screen_height(600) {
    initialPaddleX = screen_width / 2 - 75; // Assuming paddle width is 150
    initialPaddleY = screen_height - 30; // Paddle y-position
    initialBallX = screen_width / 2 - 7.5; // Assuming ball diameter is 15
    // initialBallY = initialPaddleY - 20; // Ball just above the paddle
    initialBallY = 50;
    // std::cout << "Init paddle y: " << initialPaddleY << std::endl; 

    
    // Initialize paddle and ball
    paddle = std::make_shared<Paddle>(initialPaddleX, initialPaddleY, 150, 20, 300);
    ball = std::make_shared<Ball>(initialBallX, initialBallY, 0, 400, 15);
    // std::cout << "Init ball y: " << ball->getY() << std::endl;
}

Level::~Level() {}

bool Level::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    bricks.clear(); // Clear existing bricks
    std::vector<std::string> layout;
    std::string line;
    while (std::getline(file, line)) {
        layout.push_back(line);
    }
    loadBricks(layout);
    reset();
    return true;
}

void Level::loadBricks(const std::vector<std::string>& layout) {
    int brickHeight = 20;  // Hauteur des briques
    int brickWidth = 48;   // Largeur des briques
    int brickSpacing = 2;  // Espacement entre les briques
    int yOffset = 50;      // Décalage vertical à partir du haut de l'écran

    int y = yOffset;
    for (const auto& line : layout) {  // Utilisation de 'auto' ici
        int x = 0;
        for (char ch : line) {
            if (ch == 'X') {
                bricks.push_back(std::make_shared<BasicBrick>(x, y, brickWidth, brickHeight, 1));
            }
            x += brickWidth + brickSpacing;
        }
        y += brickHeight + brickSpacing;
    }
}

void Level::update(float deltaTime) {
    ball->update(deltaTime);
    paddle->update(deltaTime);
    // Collision detection between ball and paddle
    SDL_Rect ballRect = {static_cast<int>(ball->getX()), static_cast<int>(ball->getY()), ball->getDiameter(), ball->getDiameter()};
    SDL_Rect paddleRect = {static_cast<int>(paddle->getX()), static_cast<int>(paddle->getY()), paddle->getWidth(), paddle->getHeight()};
    if (Utils::checkCollision(ballRect, paddleRect) && ball->canCollide()) {
        ball->resetCollisionTimer();
        std::cout << "Collision paddle" << std::endl;

        // Point central de la raquette
        float paddleCenter = paddle->getX() + paddle->getWidth() / 2.0;
        // Position de frappe relative au centre
        float hitPosition = (ball->getX() + ball->getDiameter() / 2.0) - paddleCenter;
        // Normalisation
        hitPosition /= (paddle->getWidth() / 2.0);

        std::cout << "Hit pos " << hitPosition << std::endl;

        // Angle de rebond ajusté pour un impact plus important
        float angle = hitPosition * (M_PI / 3.0); // Augmente l'angle jusqu'à 60 degrés de chaque côté

        std::cout << "Angle: " << angle << std::endl;

        // Calcul de la nouvelle vitesse
        float speed = hypot(ball->getVelocityX(), ball->getVelocityY());
        float newVelocityX = speed * sin(angle);
        float newVelocityY = -speed * cos(angle); // S'assurer que la balle rebondit vers le haut

        // Mise à jour de la vitesse de la balle
        ball->setVelocity(newVelocityX, newVelocityY);
    }

    for (auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            SDL_Rect brickRect = {static_cast<int>(brick->getX()), static_cast<int>(brick->getY()), brick->getWidth(), brick->getHeight()};
            if (Utils::checkCollision(ballRect, brickRect) && ball->canCollide()) {
                ball->resetCollisionTimer();
                std::string collisionSide = Utils::determineCollisionSide(ballRect, brickRect);
                brick->hit(); // Process the hit

                // Calculer le point d'impact
                float impactPointX = (ball->getX() + ball->getDiameter() / 2) - (brick->getX() + brick->getWidth() / 2);
                float normalizedImpact = impactPointX / (brick->getWidth() / 2); // Normalisé entre -1 et 1

                // Ajuster la vitesse basée sur le point d'impact
                const float MAX_ANGLE = M_PI / 4; // Angle maximum de déviation en radians
                float angleAdjustment = normalizedImpact * MAX_ANGLE;
                
                std::cout << "Collision side: " << collisionSide << std::endl;

                if (collisionSide == "TOP" || collisionSide == "BOTTOM") {
                    ball->setVelocity(ball->getVelocityX() + angleAdjustment, -ball->getVelocityY());
                } else if (collisionSide == "LEFT" || collisionSide == "RIGHT") {
                    ball->setVelocity(-ball->getVelocityX(), ball->getVelocityY() + angleAdjustment);
                }
            }
        }
    }
}

void Level::render() {
    for (const auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            brick->render(renderer);
        }
    }
    paddle->render(renderer);
    ball->render(renderer);
}

void Level::reset() {
    // Reset paddle and ball to their initial positions
    paddle->setPosition(initialPaddleX, initialPaddleY);
    ball->setPosition(initialBallX, initialBallY);
    ball->setVelocity(0, 400); // Reset ball velocity

    // Reset all bricks to not destroyed
    for (auto& brick : bricks) {
        brick->reset();
    }
}
