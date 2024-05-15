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


void Level::update(float deltaTime) {
    ball->update(deltaTime);
    paddle->update(deltaTime);

    paddle->handleCollision(*ball);

    for (auto& brick : bricks) {
        brick->handleCollision(*ball);
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
