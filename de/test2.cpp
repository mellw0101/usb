// g++ -o terminal test2.cpp `sdl2-config --cflags --libs`

#include <SDL2/SDL.h>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Terminal", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    int master, slave;
    master = posix_openpt(O_RDWR | O_NOCTTY);
    grantpt(master);
    unlockpt(master);
    slave = open(ptsname(master), O_RDWR);

    pid_t pid = fork();
    if (pid == 0) {
        // Child
        close(master);

        // Make this the controlling terminal
        setsid();
        ioctl(slave, TIOCSCTTY, 0);

        struct termios tios;
        tcgetattr(slave, &tios);
        tios.c_lflag &= ~(ECHO | ECHONL);
        tcsetattr(slave, TCSAFLUSH, &tios);

        dup2(slave, STDIN_FILENO);
        dup2(slave, STDOUT_FILENO);
        dup2(slave, STDERR_FILENO);

        execlp("/bin/sh", "/bin/sh", nullptr);
        exit(1);
    }

    // Parent
    close(slave);

    bool quit = false;
    SDL_Event e;
    char buf[128];
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                char c = e.key.keysym.sym;
                write(master, &c, 1);
            }
        }

        int n = read(master, buf, sizeof(buf));
        if (n > 0) {
            // Display the output (for this example, just print it)
            std::string output(buf, n);
            std::cout << "Output: " << output;
            // You would actually render this text to your SDL window
        }

        SDL_RenderClear(ren);
        // Render your terminal output here
        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
