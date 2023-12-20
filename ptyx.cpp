#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pty.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>

// Define a structure to hold X Window application state
struct XAppState {
    Display* display;
    Window window;
    GC gc;
    XFontStruct* fontInfo;
};

// Define a structure to hold the terminal emulator state
struct TerminalState {
    int master;
    int slave;
    pid_t child;
    char buffer[4096];
    struct termios orig_termios;
};

// Global flag for Ctrl+C handling
volatile sig_atomic_t reload_flag = 0;

// Signal handler for Ctrl+C
void handle_ctrl_c(int signum) {
    reload_flag = 1;
}

// Create the X Window
void createWindow(XAppState& appState) {
    appState.display = XOpenDisplay(nullptr);

    if (!appState.display) {
        std::cerr << "Error: Could not open X display." << std::endl;
        exit(1);
    }

    int screen = DefaultScreen(appState.display);
    appState.window = XCreateSimpleWindow(
        appState.display,
        RootWindow(appState.display, screen),
        10, 10, 400, 200, 1,
        BlackPixel(appState.display, screen),
        WhitePixel(appState.display, screen)
    );

    XSelectInput(appState.display, appState.window, KeyPressMask | KeyReleaseMask | ExposureMask);
    XMapWindow(appState.display, appState.window);

    XGCValues values;
    values.foreground = BlackPixel(appState.display, screen);
    values.background = WhitePixel(appState.display, screen);
    appState.gc = XCreateGC(appState.display, appState.window, GCForeground | GCBackground, &values);

    appState.fontInfo = XLoadQueryFont(appState.display, "fixed");
    if (!appState.fontInfo) {
        std::cerr << "Error: Could not load font." << std::endl;
        exit(1);
    }
    XSetFont(appState.display, appState.gc, appState.fontInfo->fid);
}

// Draw text in the X Window
void drawText(XAppState& appState, const std::string& text) {
    XClearWindow(appState.display, appState.window);
    int x = 10;
    int y = 30;
    XDrawString(appState.display, appState.window, appState.gc, x, y, text.c_str(), text.length());
}

// Handle keyboard input and drawing in X Window
void eventLoop(XAppState& appState, TerminalState& termState) {
    XEvent event;
    std::string text;

    while (true) {
        XNextEvent(appState.display, &event);

        if (event.type == KeyPress) {
            char buffer[32];
            KeySym key;
            XLookupString(&event.xkey, buffer, sizeof(buffer), &key, nullptr);

            if (key == XK_Return) {
                std::cout << "Typed: " << text << std::endl;
                text.clear();
            } else if (key == XK_BackSpace && !text.empty()) {
                text.pop_back();
            } else if (key >= XK_space && key <= XK_asciitilde) {
                text += buffer;
            }

            drawText(appState, text);
        } else if (event.type == Expose) {
            drawText(appState, text);
        }
    }
}

// Create the terminal emulator
void createTerminal(TerminalState& termState) {
    if (openpty(&termState.master, &termState.slave, NULL, NULL, NULL) == -1) {
        perror("openpty");
        exit(1);
    }

    termState.child = fork();

    if (termState.child == -1) {
        perror("fork");
        exit(1);
    } else if (termState.child == 0) {
        close(termState.master);
        dup2(termState.slave, STDIN_FILENO);
        dup2(termState.slave, STDOUT_FILENO);
        dup2(termState.slave, STDERR_FILENO);
        close(termState.slave);

        execl("/bin/Shell", "/bin/Shell", NULL); // Replace with the shell of your choice
    } else {
        close(termState.slave);

        struct termios new_termios;
        tcgetattr(STDIN_FILENO, &(termState.orig_termios));
        new_termios = termState.orig_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

        int flags = fcntl(termState.master, F_GETFL, 0);
        fcntl(termState.master, F_SETFL, flags | O_NONBLOCK);
    }
}

// Handle terminal input and output
void terminalLoop(XAppState& appState, TerminalState& termState) {
    while (true) {
        XAppState tempAppState = appState; // Create a temporary copy for X Window events
        eventLoop(tempAppState, termState);

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(termState.master, &readfds);

        if (select(termState.master + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            int bytesRead = read(STDIN_FILENO, termState.buffer, sizeof(termState.buffer));
            if (bytesRead <= 0) {
                break;
            }

            if (bytesRead == 3 && termState.buffer[0] == 27 && termState.buffer[1] == 91) {
                // Handle special keys here (e.g., up and down arrow keys)
                // You may implement specific behavior for these keys.
                // For simplicity, we're just sending them as is to the PTY.
            }

            write(termState.master, termState.buffer, bytesRead);
        }

        if (FD_ISSET(termState.master, &readfds)) {
            int bytesRead = read(termState.master, termState.buffer, sizeof(termState.buffer));
            if (bytesRead <= 0) {
                break;
            }
            write(STDOUT_FILENO, termState.buffer, bytesRead);
        }

        if (reload_flag) {
            // Handle Ctrl+C (restart terminal)
            reload_flag = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &(termState.orig_termios)); // Restore terminal attributes
            createTerminal(termState); // Recreate the terminal
        } else {
            break; // Exit the program
        }
    }
}

int main() {
    XAppState appState;
    createWindow(appState);

    TerminalState termState;
    createTerminal(termState);

    // Set up signal handling for Ctrl+C
    struct sigaction sa;
    sa.sa_handler = handle_ctrl_c;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    terminalLoop(appState, termState);

    // Clean up X Window resources
    XFreeFontInfo(nullptr, appState.fontInfo, 1);
    XFreeGC(appState.display, appState.gc);
    XDestroyWindow(appState.display, appState.window);
    XCloseDisplay(appState.display);

    return 0;
}
