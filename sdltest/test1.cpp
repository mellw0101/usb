#include <SDL2/SDL.h> // Include the SDL2 header
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void renderer(SDL_Window* window) {
    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the background color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize: %s", SDL_GetError());
        return 1;
    }

    // Create an SDL2 window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",            // Window title
        SDL_WINDOWPOS_CENTERED,  // Initial x position
        SDL_WINDOWPOS_CENTERED,  // Initial y position
        640,                      // Width
        480,                      // Height
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE        // Flags (e.g., SDL_WINDOW_SHOWN, SDL_WINDOW_FULLSCREEN)
    );

    // Check if the window was created successfully
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // renderer(window);
    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the background color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Event loop
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Handle button click
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    // Check if the click is within the button's area (example: button at (100, 100) with a size of 100x40)
                    if (mouseX >= 100 && mouseX <= 200 && mouseY >= 100 && mouseY <= 140) {
                        // Button clicked, perform action here
                        SDL_Log("Button clicked!");
                    }
                }
            }
        }

        // Render the button (for simplicity, just a filled rectangle)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red button color
        SDL_Rect buttonRect = {100, 100, 100, 40}; // Button position and size
        SDL_RenderFillRect(renderer, &buttonRect);
        SDL_RenderPresent(renderer);
    }

    // Cleanup and quit SDL2
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
