#pragma once

#include <vector>
#include <memory>
#include <string>
#include <random>
#include "bricks/BasicBrick.h"
#include "bricks/HexagonalBrick.h"
#include "bricks/TriangularBrick.h"
#include "Paddle.h"
#include "Ball.h"
#include <SDL.h>


// Level class for managing the game levels in Breakout
class Level {
public:
    enum class BrickType {
        Basic,
        Hexagonal,
        Triangular
    };

    Level(SDL_Renderer* renderer);
    ~Level();

    // Functions to manage the level
    bool loadLevel(const std::string& filename); // Load level from an ASCII file
    bool update(float deltaTime, bool& victory); // Update all objects in the level
    void render(); // Render all objects in the level
    void reset(); // Reset the level to initial state

    void setBrickType(BrickType type) { currentBrickType = type; }

private:
    SDL_Renderer* renderer; // SDL Renderer for drawing
    std::vector<std::shared_ptr<Brick>> bricks;
    std::shared_ptr<Paddle> paddle; // Player's paddle
    std::vector<std::shared_ptr<Ball>> balls; // The balls

    BrickType currentBrickType = BrickType::Basic;

    void loadBricks(const std::vector<std::string>& layout);
    void loadBasicBricks(const std::vector<std::string>& layout);
    void loadHexagonalBricks();
    void loadTriangularBricks();
    void addNewBall(float x, float y, float velX, float velY);
    void triggerMultiBall();
    void activatePaddleExpansion(float duration);
    

    // Initial settings
    float initialPaddleX, initialPaddleY;
    float initialBallX, initialBallY;

    int screen_width, screen_height;
    bool isPaddleExpanded;
    float paddleExpansionEndTime;
};
