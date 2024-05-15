#include "Level.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

Level::Level(SDL_Renderer* renderer)
    : renderer(renderer), screen_width(800), screen_height(600) {
    initialPaddleX = screen_width / 2 - 75; // Assuming paddle width is 150
    initialPaddleY = screen_height - 30; // Paddle y-position
    initialBallX = screen_width / 2 - 7.5; // Assuming ball diameter is 15
    initialBallY = initialPaddleY - 180; // Ball just above the paddle
    
    // Initialize paddle and ball
    paddle = std::make_shared<Paddle>(initialPaddleX, initialPaddleY, 150, 20, 400);

    // Initialize the ball vector with one ball
    std::shared_ptr<Ball> initialBall = std::make_shared<Ball>(initialBallX, initialBallY, 0, 400, 15);
    balls.push_back(initialBall);
}

Level::~Level() {}


bool Level::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File doesn't exist \n";
        return false;
    }

    bricks.clear(); // Clear existing bricks
    std::vector<std::string> layout;
    std::string line;
    while (std::getline(file, line)) {
        layout.push_back(line);
    }
    
    switch (currentBrickType) {
        case BrickType::Basic:
            loadBasicBricks(layout);
            break;
        case BrickType::Hexagonal:
            loadHexagonalBricks();
            break;
        case BrickType::Triangular:
            loadTriangularBricks();
            break;
    }

    reset();
    return true;
}

void Level::loadBasicBricks(const std::vector<std::string>& layout) {
    int brickHeight = 20;  // Height of bricks
    int brickWidth = 48;   // Width of bricks
    int brickSpacing = 2;  // Spacing between bricks
    int yOffset = 50;      // Vertical offset from the top of the screen

    int y = yOffset;
    int totalBricks = 0;

    for (const auto& line : layout) {
        int x = 0;
        for (char ch : line) {
            if (ch == 'X') {
                int hitPoints = 1;
                if (totalBricks % 10 == 0) {
                    hitPoints = 3;
                } else if (totalBricks % 8 == 0) {
                    hitPoints = 2;
                }
                bricks.push_back(std::make_shared<BasicBrick>(x, y, brickWidth, brickHeight, hitPoints));
                ++totalBricks;
            }
            x += brickWidth + brickSpacing;
        }
        y += brickHeight + brickSpacing;
    }
}

void Level::loadHexagonalBricks() {
    float sideLength = 30; // Side length of the hexagon
    int rows = 5; // Number of rows of hexagons
    int columns = 17; // Number of columns of hexagons
    float height = sqrt(3) * sideLength; // Height of the hexagon
    float width = 2 * sideLength; // Width of the hexagon
    float verticalSpacing = height; // Vertical distance between hexagons in a column
    float horizontalSpacing = width * 0.75f; // Horizontal distance between hexagons in a row

    float xOffset = 40; // Starting offset for x
    float yOffset = 50; // Starting offset for y

    int totalBricks = 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            // Calculate the position of the hexagon
            float x = col * horizontalSpacing + xOffset;
            float y = row * verticalSpacing + yOffset;

            // Offset every second hexagon in the row vertically
            if (col % 2 != 0) {
                y += height / 2;
            }

            int hitPoints = 1;
            if (totalBricks % 10 == 0) {
                hitPoints = 3;
            } else if (totalBricks % 8 == 0) {
                hitPoints = 2;
            }

            // Create the hexagonal brick
            bricks.push_back(std::make_shared<HexagonalBrick>(x, y, sideLength, hitPoints));
            ++totalBricks;
        }
    }
}

void Level::loadTriangularBricks() {
    float base = 50; // Base of the triangle
    float height = sqrt(3) / 2 * base; // Height of the triangle
    int rows = 5; // Number of rows of triangles
    int columns = 28; // Number of columns of triangles

    float xOffset = 40; // Starting offset for x
    float yOffset = 50; // Starting offset for y

    int totalBricks = 0;

    for (int row = 0; row < rows; ++row) {
        bool rowStartsInverted = (row % 2 != 0);
        for (int col = 0; col < columns; ++col) {
            float x = col * (base / 2) + xOffset;
            float y = row * height + yOffset;

            bool inverted = rowStartsInverted ? (col % 2 == 0) : (col % 2 != 0);

            int hitPoints = 1;
            if (totalBricks % 10 == 0) {
                hitPoints = 3;
            } else if (totalBricks % 8 == 0) {
                hitPoints = 2;
            }

            // Create the triangular brick
            bricks.push_back(std::make_shared<TriangularBrick>(x, y, base, height, inverted, hitPoints));
            ++totalBricks;
        }
    }
}


bool Level::update(float deltaTime, bool& victory) {
    int allDestroyed = true;
    for (auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            allDestroyed = false;
            break;
        }
    }

    if (allDestroyed) {
        victory = true;
        return false;
    }


    float currentTime = SDL_GetTicks() / 1000.0f;
    // Handle paddle expansion duration
    if (isPaddleExpanded && currentTime >= paddleExpansionEndTime) {
        paddle->setWidth(paddle->getWidth() - 30);  // Reset to original width
        isPaddleExpanded = false;
    }

    for (auto it = balls.begin(); it != balls.end(); ) {
        auto ball = *it;
        if (ball) {
            ball->update(deltaTime);

            if (ball->getY() > paddle->getY()) {
                it = balls.erase(it); // Remove ball if it falls below the paddle
            } else {
                ++it;
            }
        }
        else {
            std::cerr << "Encountered a null ball pointer." << std::endl;
            it = balls.erase(it);
        }
    }

    paddle->update(deltaTime);

    if (balls.empty()) {
        return false;
    }

    // Check for collisions between each ball and the paddle
    for (auto& ball : balls) {
        paddle->handleCollision(*ball);
    }

    bool addBalls = false;
    bool expandPaddle = false;
    // Check for collisions between each ball and each brick
    for (auto& brick : bricks) {
        for (auto& ball : balls) {
            brick->handleCollision(*ball, addBalls, expandPaddle);
        }
    }

    if (addBalls) {
        triggerMultiBall();
    }

    if (expandPaddle) {
        activatePaddleExpansion(20);
    }
    
    return true;
}

void Level::render() {
    for (const auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            brick->render(renderer);
        }
    }
    paddle->render(renderer);

    for (auto& ball : balls) {
        ball->render(renderer);
    }
}

void Level::reset() {
    // Reset paddle and ball to their initial positions
    paddle->setPosition(initialPaddleX, initialPaddleY);
    for (auto& ball : balls) {
        ball->setPosition(initialBallX, initialBallY);
        ball->setVelocity(0, 400); // Reset ball velocity
    }

    // Reset all bricks to not destroyed
    // for (auto& brick : bricks) {
    //     brick->reset();
    // }
}

void Level::addNewBall(float x, float y, float velX, float velY) {
    auto newBall = std::make_shared<Ball>(x, y, velX, velY, 15); // Assuming ball diameter is 15
    balls.push_back(newBall);
}

void Level::triggerMultiBall() {
    if (balls.empty()) return; // Ensure there's at least one ball to duplicate

    auto baseBall = balls.front();
    addNewBall(baseBall->getX(), baseBall->getY(), baseBall->getVelocityX() * 1.1f, -baseBall->getVelocityY());
    addNewBall(baseBall->getX(), baseBall->getY(), baseBall->getVelocityX() * 0.9f, -baseBall->getVelocityY());
}

void Level::activatePaddleExpansion(float duration) {
    paddle->setWidth(paddle->getWidth() + 30);  // Expand the paddle
    isPaddleExpanded = true;
    paddleExpansionEndTime = SDL_GetTicks() / 1000.0f + duration;  // Set the end time for the bonus
}