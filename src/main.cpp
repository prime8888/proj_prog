#include <SDL.h>
#include "Game.h"

int main(int argc, char* argv[]) {
    Game breakoutGame;

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "Unable to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Initialize the game
    breakoutGame.init("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

    

    // Main game loop
    const int FPS = 60;
    const int frameDelay = 300 / FPS;

    Uint32 frameStart;
    int frameTime = 0;

    while (breakoutGame.running()) {
        frameStart = SDL_GetTicks();

        // Handle input from the user
        breakoutGame.handleEvents();

        // Update game logic
        float deltaTime = frameTime / 1000.0f; // Convert frameTime to seconds
        breakoutGame.update(deltaTime);

        // Render the game to the screen
        breakoutGame.render();

        frameTime = SDL_GetTicks() - frameStart;
        
        // Delay to cap the frame rate
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up resources
    breakoutGame.clean();

    return 0;
}
