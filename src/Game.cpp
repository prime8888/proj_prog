#include "Game.h"
#include <iostream>

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
    level->update(deltaTime);
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
    level->setBrickType(Level::BrickType::Triangular);
    level->loadLevel("../levels/level1.txt");
}

