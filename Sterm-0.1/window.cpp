#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <regex>
#include <stdexcept>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>
#include <sstream>

bool initSDL() {
    return SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() == 0;
}
SDL_Window* createWindow(const std::string& title = "Terminal Emulator",
                         int x = SDL_WINDOWPOS_UNDEFINED,
                         int y = SDL_WINDOWPOS_UNDEFINED,
                         int w = 1200,
                         int h = 800,
                         Uint32 flags = SDL_WINDOW_SHOWN) {
    SDL_Window* window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        // Handle error as appropriate.
    }
    return window;
}
SDL_Renderer* createRenderer(SDL_Window* window, int index = -1, Uint32 flags = SDL_RENDERER_ACCELERATED) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        // Handle error as appropriate.
    }
    return renderer;
}
class TerminalConfigurator {
public:
    static void configure(int fd) {
        struct termios term;
        if (tcgetattr(fd, &term) == -1) {
            throw std::runtime_error("Failed to get terminal attributes.");
        }

        term.c_lflag &= ~(ECHO);
        term.c_lflag &= ~(ICANON);
        term.c_cc[VMIN] = 1;
        term.c_cc[VTIME] = 0;

        if (tcsetattr(fd, TCSANOW, &term) == -1) {
            throw std::runtime_error("Failed to set terminal attributes.");
        }
    }
};
int initPtyMaster() {
    int master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (master_fd == -1) {
        throw std::runtime_error("Failed to open PTY master.");
    }
    if (grantpt(master_fd) == -1 || unlockpt(master_fd) == -1) {
        close(master_fd);
        throw std::runtime_error("Failed to grant or unlock PTY master.");
    }

    char* slave_name = ptsname(master_fd);
    if (slave_name == nullptr) {
        close(master_fd);
        throw std::runtime_error("Failed to get PTY slave name.");
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(master_fd);
        throw std::runtime_error("Fork failed.");
    } else if (pid == 0) {
        setsid();
        close(master_fd);

        int slave_fd = open(slave_name, O_RDWR);
        if (slave_fd == -1) {
            exit(1);
        }

        TerminalConfigurator::configure(slave_fd);

        dup2(slave_fd, STDIN_FILENO);
        dup2(slave_fd, STDOUT_FILENO);
        dup2(slave_fd, STDERR_FILENO);
        close(slave_fd);

        if (execlp("/bin/sh", "/bin/sh", NULL) == -1) {
            exit(1);
        }
        exit(1);
    }

    return master_fd;
}
std::string getPtySlaveName(int master_fd) {
    char *name = ptsname(master_fd);
    if (name == nullptr) {
        throw std::runtime_error("Failed to get PTY slave name.");
    }
    return std::string(name);
}

struct CharInfo {
    SDL_Rect srcRect;
    int w, h;
};
SDL_Texture* textureAtlas = nullptr;
std::unordered_map<char, CharInfo> charMap;
void generateTextureAtlas(SDL_Renderer* renderer, TTF_Font* const font) {
    std::string allChars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    int textureWidth = 0, textureHeight = 0;

    for (char c : allChars) {
        SDL_Surface* charSurface = TTF_RenderGlyph_Solid(font, c, {255, 255, 255, 255});
        if (!charSurface) continue;

        CharInfo info = { {0, 0, charSurface->w, charSurface->h} };
        textureWidth += info.srcRect.w;
        textureHeight = std::max(textureHeight, info.srcRect.h);

        charMap[c] = info;

        SDL_FreeSurface(charSurface);
    }

    textureAtlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, textureHeight);
    if (!textureAtlas) {
        std::cerr << "Failed to create texture: " << SDL_GetError();
        return;
    }

    int x = 0;
    for (char c : allChars) {
        CharInfo& info = charMap[c];  // Using a reference for simplicity
        SDL_Surface* charSurface = TTF_RenderGlyph_Solid(font, c, {255, 255, 255, 255});
        if (!charSurface) continue;

        SDL_Texture* charTexture = SDL_CreateTextureFromSurface(renderer, charSurface);
        if (!charTexture) {
            SDL_FreeSurface(charSurface);
            continue;
        }

        info.srcRect.x = x;
        SDL_SetRenderTarget(renderer, textureAtlas);
        SDL_RenderCopy(renderer, charTexture, NULL, &info.srcRect);
        SDL_SetRenderTarget(renderer, NULL);

        x += info.srcRect.w;

        SDL_DestroyTexture(charTexture);
        SDL_FreeSurface(charSurface);
    }
}
void renderText(SDL_Renderer* renderer, TTF_Font* const font, const std::string& text, const SDL_Color& color, int x, int y, int maxWidth) {
    if (text.empty()) return;

    int curX = x, curY = y;
    int lineSkip = TTF_FontLineSkip(font);

    for (char c : text) {
        if (charMap.find(c) == charMap.end()) continue;

        const CharInfo& charInfo = charMap[c];
        int newWidth = curX + charInfo.srcRect.w;

        if (newWidth > x + maxWidth) {
            curY += lineSkip;
            curX = x;
        }

        SDL_Rect dstRect = {curX, curY, charInfo.srcRect.w, charInfo.srcRect.h};
        SDL_RenderCopy(renderer, textureAtlas, &charInfo.srcRect, &dstRect);

        curX = newWidth;
    }
}

std::string trimToVisibleLines(const std::string& str, size_t numLines) {
    if (str.empty() || numLines == 0) return "";

    size_t pos = str.size() - 1;
    for (; pos > 0; --pos) {
        if (str[pos] == '\n') {
            if (--numLines == 0) {
                return str.substr(pos + 1);
            }
        }
    }

    return str;
}
std::string removeAnsiCodes(const std::string& text) {
    std::string cleanedText;
    cleanedText.reserve(text.size());  // Reserve space to minimize reallocations

    bool inEscapeCode = false;

    for (char c : text) {
        if (c == '\x1b' || c == '\033' || c == '\e') {
            inEscapeCode = true;
        } else if (!inEscapeCode) {
            cleanedText.push_back(c);
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            inEscapeCode = false;
        }
    }

    cleanedText.shrink_to_fit(); // Free up any unused capacity
    return cleanedText;
}
void vt102Emulate(const std::string &input) {
    std::vector<std::vector<char>> screen(24, std::vector<char>(80, ' '));  // 24 rows x 80 columns
    int row = 0, col = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '\033') {  // ESC
            ++i;  // Skip the '[' after '\033'
            if (i < input.size() && input[i] == '[') {
                ++i;  // Move to the next character

                std::string code;
                while (i < input.size() && input[i] >= '0' && input[i] <= '9') {
                    code += input[i];
                    ++i;
                }

                if (i < input.size()) {
                    char cmd = input[i];
                    if (cmd == 'A') {
                        // Move cursor up
                        row -= std::stoi(code.empty() ? "1" : code);
                        if (row < 0) row = 0;
                    } else if (cmd == 'B') {
                        // Move cursor down
                        row += std::stoi(code.empty() ? "1" : code);
                        if (row >= 24) row = 23;
                    } else if (cmd == 'C') {
                        // Move cursor forward
                        col += std::stoi(code.empty() ? "1" : code);
                        if (col >= 80) col = 79;
                    } else if (cmd == 'D') {
                        // Move cursor backward
                        col -= std::stoi(code.empty() ? "1" : code);
                        if (col < 0) col = 0;
                    }
                }
            }
        } else if (c == '\n') {
            // Newline
            ++row;
            if (row >= 24) row = 23;
            col = 0;
        } else if (c == '\r') {
            // Carriage return
            col = 0;
        } else {
            // Regular character
            if (row < 24 && col < 80) {
                screen[row][col] = c;
            }
            ++col;
            if (col >= 80) {
                col = 0;
                ++row;
                if (row >= 24) row = 23;
            }
        }
    }

    // Just to display the "screen" for demonstration purposes
    for (const auto &line : screen) {
        for (char c : line) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}
void handleAnsiCodes(const std::string &input, std::vector<std::vector<char>> &screen, int &row, int &col) {
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '\033') {  // ESC
            ++i;  // Skip the '[' after '\033'
            if (i < input.size() && input[i] == '[') {
                ++i;  // Move to the next character

                std::string code;
                while (i < input.size() && input[i] >= '0' && input[i] <= '9') {
                    code += input[i];
                    ++i;
                }

                if (i < input.size()) {
                    char cmd = input[i];
                    int num = std::stoi(code.empty() ? "1" : code);

                    if (cmd == 'A') {  // Move cursor up
                        row -= num;
                        if (row < 0) row = 0;
                    } else if (cmd == 'B') {  // Move cursor down
                        row += num;
                        if (row >= 24) row = 23;
                    } else if (cmd == 'C') {  // Move cursor forward
                        col += num;
                        if (col >= 80) col = 79;
                    } else if (cmd == 'D') {  // Move cursor backward
                        col -= num;
                        if (col < 0) col = 0;
                    }
                }
            }
        } else {
            if (row < 24 && col < 80) {
                screen[row][col] = c;
            }
            ++col;
            if (col >= 80) {
                col = 0;
                ++row;
                if (row >= 24) row = 23;
            }
        }
    }
}
void mainLoop(SDL_Renderer* renderer, int master_fd) {
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono.ttf", 14); // replace with your font path and size
    generateTextureAtlas(renderer, font);  // Generate texture atlas
    // Preload textures just after font initialization
    bool running = true;
    SDL_Event event;
    char buffer[1024];
    std::string terminalOutput;
    std::string userTypedText; // Outside the while loop to keep the state

    const int lineHeight = 14; // Height of one line, in pixels
    const int maxLines = 56;   // Maximum number of lines to display

    while (running) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(master_fd, &readfds);
        struct timeval tv = { 0, 50000 }; // 50 ms timeout

        if (select(master_fd + 1, &readfds, NULL, NULL, &tv) > 0) {
            ssize_t count = read(master_fd, buffer, sizeof(buffer) - 1);
            if (count > 0) {
                buffer[count] = '\0';
                for(int i = 0; i < count; ++i) {
                    if (isprint(buffer[i]) || buffer[i] == '\n' || buffer[i] == '\t') {
                        terminalOutput += buffer[i];
                    }
                }
                // terminalOutput = removeAnsiCodes(terminalOutput);
                // vt102Emulate(terminalOutput);
                // int row = 0, col = 0;  // Initialize these wherever makes sense in your code
                // handleAnsiCodes(terminalOutput, screen, row, col);
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                int ch = static_cast<int>(event.key.keysym.sym);
                Uint16 mod = event.key.keysym.mod;
                // Detect if shift is held down
                bool isShiftDown = (mod & KMOD_SHIFT) != 0;
                if (ch == SDLK_RETURN) {
                    char lf = '\n';
                    write(master_fd, &lf, 1);
                    terminalOutput += userTypedText + '\n'; // Add your prompt and user input to terminal output
                    userTypedText.clear(); // Clear the user input
                } else if (ch == SDLK_BACKSPACE) {
                    if (!userTypedText.empty()) {
                        userTypedText.pop_back(); // Remove the last character from user input
                    }
                    write(master_fd, "\b \b", 3); // Send backspace to terminal to clear last character
                } else if (ch < 128) {
                    char writeCh = static_cast<char>(ch);
                    if (isShiftDown && writeCh >= 'a' && writeCh <= 'z') {
                        writeCh -= 'a' - 'A';
                    }
                    write(master_fd, &writeCh, 1);
                    userTypedText += writeCh; // Add the character to user input
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        std::list<std::string> lines;
        size_t pos = 0;
        std::string current_line;

        for (char c : terminalOutput) {
            if (c == '\n') {
                lines.push_back(current_line);
                current_line.clear();
            } else {
                current_line += c;
            }
        }
        if (!current_line.empty()) {
            lines.push_back(current_line);
            current_line.clear();
        }

        if (!lines.empty()) {
            lines.back() += userTypedText;
        }

        while (lines.size() > maxLines) {
            lines.pop_front();
        }

        SDL_Color textColor = {255, 255, 255, 255};
        int y = 786;
        int x = 4; // Add this line at a suitable location before the while loop or inside it, as per your needs

        for (auto it = lines.rbegin(); it != lines.rend(); ++it, y -= lineHeight) {
            renderText(renderer, font, *it, textColor, x, y, 1200); // Use the new renderText
        }

        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
}
int main(int argc, char *argv[]) {
    if (!initSDL()) return 1;

    SDL_Window* window = createWindow();
    if (!window) return 1;

    SDL_Renderer* renderer = createRenderer(window);
    if (!renderer) return 1;

    int master_fd = initPtyMaster();
    if (master_fd == -1) return 1;

    std::string slave_name = getPtySlaveName(master_fd);
    if (slave_name.empty()) return 1;

    mainLoop(renderer, master_fd);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    close(master_fd);
    SDL_Quit();

    return 0;
}
