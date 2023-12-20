#include <SDL2/SDL.h>
#include <iostream>

// g++ -o dock dock.cpp `sdl2-config --cflags --libs`



bool InitializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Window* CreateDockWindow(const char* title, int width, int height) {
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    int screen_height = DM.h;

    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = screen_height - height;

    SDL_Window* window = SDL_CreateWindow(
        title,
        x,
        y,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    }

    return window;
}

void RenderDock(SDL_Renderer* renderer, int dockWidth, int dockHeight) {
    SDL_Rect dockRect = {0, 0, dockWidth, dockHeight};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &dockRect);
}

int main(int argc, char* argv[]) {
    if (!InitializeSDL()) {
        return 1;
    }

    int dockWidth = 3840;
    int dockHeight = 50;

    SDL_Window* window = CreateDockWindow("Dock", dockWidth, dockHeight);

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
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << "Dock clicked!" << std::endl;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        RenderDock(renderer, dockWidth, dockHeight);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
