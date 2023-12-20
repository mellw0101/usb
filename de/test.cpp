#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <functional>

// g++ -o fullscreen test.cpp `sdl2-config --cflags --libs`
bool InitializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Window* CreateFullscreenWindow(const char* title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_FULLSCREEN
    );

    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    }

    return window;
}

void RenderDock(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    SDL_Rect dockRect;
    dockRect.x = 0;
    dockRect.y = screenHeight - 50;
    dockRect.w = screenWidth;
    dockRect.h = 50;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &dockRect);
}

bool IsMouseInDock(int mouseY, int screenHeight) {
    return mouseY >= screenHeight - 50;
}

int main(int argc, char* argv[]) {
    if (!InitializeSDL()) {
        return 1;
    }

    int screenWidth = 3840;
    int screenHeight = 1200;

    SDL_Window* window = CreateFullscreenWindow("Fullscreen Window with Dock", screenWidth, screenHeight);

    if (window == nullptr) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q && (SDL_GetModState() & KMOD_CTRL)) {
                    running = false;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (IsMouseInDock(event.button.y, screenHeight)) {
                    std::cout << "Dock clicked!" << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderDock(renderer, screenWidth, screenHeight);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

