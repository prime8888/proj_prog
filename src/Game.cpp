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

        isRunning = true;
        setupLevel();
    } else {
        isRunning = false;
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }
}

void Game::update(float deltaTime) {
    bool victory = false;
    bool res = level->update(deltaTime, victory);
    if (!res) {
        isRunning = false;
        TTF_Font* font = TTF_OpenFont("../fonts/RubikMonoOne-Regular.ttf", 40);

        if (victory) {
            displayExitMessage(font, "Victory !");
        }
        else {
            displayExitMessage(font, "Game Over");
        }
        // Wait for a few seconds
        SDL_Delay(5000);
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    level->render();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned.\n";
}

void Game::setupLevel() {
    level = std::make_unique<Level>(renderer);
    //level->loadLevel("../levels/easy_test.txt");
    level->loadLevel("../levels/level1.txt");
}

void Game::displayExitMessage(TTF_Font* font, const std::string& message) {
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

