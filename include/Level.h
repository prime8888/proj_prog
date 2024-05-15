#pragma once

#include <vector>
#include <memory>
#include <string>
#include "bricks/BasicBrick.h"
#include "Paddle.h"
#include "Ball.h"
#include <SDL.h>

// Level class for managing the game levels in Breakout
class Level {
public:
    Level(SDL_Renderer* renderer);
    ~Level();

    // Functions to manage the level
    bool loadLevel(const std::string& filename); // Load level from an ASCII file
    bool update(float deltaTime, bool& victory); // Update all objects in the level
    void render(); // Render all objects in the level
    void reset(); // Reset the level to initial state

private:
    SDL_Renderer* renderer; // SDL Renderer for drawing
    std::vector<std::shared_ptr<BasicBrick>> bricks; // List of bricks
    std::shared_ptr<Paddle> paddle; // Player's paddle
    std::shared_ptr<Ball> ball; // The ball

    // Load bricks from file data
    void loadBricks(const std::vector<std::string>& layout);

    // Initial settings
    float initialPaddleX, initialPaddleY;
    float initialBallX, initialBallY;
    int screen_width, screen_height;
};
