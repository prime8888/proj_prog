#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class Menu {
public:
    Menu(SDL_Renderer* renderer, TTF_Font* font);
    ~Menu();

    void handleEvents(SDL_Event& e);
    void render();
    bool isRunning() const;
    std::string getSelectedOption() const;

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;
    int selectedOption;
    std::vector<std::string> options;

    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif