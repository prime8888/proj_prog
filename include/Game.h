#pragma once

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <iostream>
#include "Level.h"
#include "Menu.h"

// Game class for managing the state and loop of the Breakout game
class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();

    bool running() const { return isRunning; }

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Level> level;

    TTF_Font* font;
    std::unique_ptr<Menu> menu;
    enum GameState { MENU, GAME } gameState;

    void setupLevel(Level::BrickType type, int levelNumber);
    void displayExitMessage(const std::string& message);
};
