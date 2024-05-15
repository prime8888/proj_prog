#include "Game.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) {}

Game::~Game() {
    clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL Initialized.\n";


        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created.\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created.\n";
        }

        font = TTF_OpenFont("../fonts/RubikMonoOne-Regular.ttf", 40);

        isRunning = true;
        gameState = MENU;
        menu = std::make_unique<Menu>(renderer, font);
        level = nullptr;
        // setupLevel();
    } else {
        isRunning = false;
    }
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (gameState == MENU) {
        menu->handleEvents(event);
        if (!menu->isRunning()) {
            std::string option = menu->getSelectedOption();
            if (option == "Basic - Level 1") {
                setupLevel(Level::BrickType::Basic, 1);
            } else if (option == "Basic - Level 2") {
                setupLevel(Level::BrickType::Basic, 2);
            } else if (option == "Basic - Level 3") {
                setupLevel(Level::BrickType::Basic, 3);
            } else if (option == "Hexagonal Layout") {
                setupLevel(Level::BrickType::Hexagonal, 1);
            } else if (option == "Triangular Layout") {
                setupLevel(Level::BrickType::Triangular, 1);
            } else if (option == "Quit") {
                isRunning = false;
            }
            gameState = GAME;
        }
    } else if (gameState == GAME) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Game::update(float deltaTime) {
    if (gameState == GAME && level) {
        bool victory = false;
        bool res = level->update(deltaTime, victory);
        if (!res) {
            isRunning = false;

            if (victory) {
                displayExitMessage("Victory !");
            }
            else {
                displayExitMessage("Game Over");
            }
            // Wait for a few seconds
            SDL_Delay(4000);
        }
        
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    if (gameState == MENU) {
        menu->render();
    } else if (gameState == GAME && level) {
        level->render();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned.\n";
}

void Game::setupLevel(Level::BrickType type, int levelNumber) {
    level = std::make_unique<Level>(renderer);
    level->setBrickType(type);
    const std::string filePath = "../levels/level" + std::to_string(levelNumber) + ".txt";
    level->loadLevel(filePath);
}

void Game::displayExitMessage(const std::string& message) {
    // Render a rectangle for the message background
    SDL_Rect rect = {200, 200, 400, 200}; // Position and size of the rectangle
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255); // Set color to grey
    SDL_RenderFillRect(renderer, &rect); // Render the rectangle

    // Render text inside the rectangle
    SDL_Color textColor = {0, 0, 0}; // Black color
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), textColor); // Render text surface
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); // Create texture from surface
    SDL_Rect textRect = {250, 275, textSurface->w, textSurface->h}; // Position of the text
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Render text to the screen

    // Cleanup resources
    SDL_FreeSurface(textSurface); // Free surface
    SDL_DestroyTexture(textTexture); // Destroy texture

    // Present the rendered content
    SDL_RenderPresent(renderer); // Present the renderer
}

