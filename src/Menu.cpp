#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), running(true), selectedOption(0) {
    options = {
        "Basic - Level 1",
        "Basic - Level 2",
        "Basic - Level 3",
        "Hexagonal Layout",
        "Triangular Layout",
        "Quit"
    };
}

Menu::~Menu() {}

void Menu::handleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        running = false;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = (selectedOption > 0) ? selectedOption - 1 : options.size() - 1;
                break;
            case SDLK_DOWN:
                selectedOption = (selectedOption < options.size() - 1) ? selectedOption + 1 : 0;
                break;
            case SDLK_RETURN:
                running = false;
                break;
            default:
                break;
        }
    }
}

void Menu::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < options.size(); ++i) {
        SDL_Color color = (i == selectedOption) ? SDL_Color{255, 255, 255, 255} : SDL_Color{128, 128, 128, 255};
        renderText(options[i], 100, 100 + static_cast<int>(i) * 50, color);
    }

    SDL_RenderPresent(renderer);
}

bool Menu::isRunning() const {
    return running;
}

std::string Menu::getSelectedOption() const {
    return options[selectedOption];
}

void Menu::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}