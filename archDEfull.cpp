#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


// Function to start and display a program in an XTerm window
void startProgramInXTerm(Display* display, Window window, const char* programPath) {
    // Construct the command to run in XTerm
    std::string command = "xterm -into " + std::to_string(window) + " -e " + programPath;

    // Fork a child process to execute the XTerm command
    pid_t pid = fork();
    if (pid == 0) {
        // This code runs in the child process
        execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
        std::cerr << "Error: Could not execute the program. Error code: " << strerror(errno) << std::endl;
        exit(1);
    } else if (pid > 0) {
        // This code runs in the parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Error: Fork failed. Error code: " << strerror(errno) << std::endl;
    }
}

// Function to open the shell or any other program
void openProgram(const char* programPath) {
    pid_t pid = fork();
    if (pid == 0) {
        // This code runs in the child process
        execlp(programPath, programPath, NULL);
        std::cerr << "Error: Could not open the program. Error code: " << strerror(errno) << std::endl;
        exit(1);
    } else if (pid > 0) {
        // This code runs in the parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Error: Fork failed. Error code: " << strerror(errno) << std::endl;
    }
}

void drawRectangle(Display* display, Window window, GC gc) {
    XSetForeground(display, gc, WhitePixel(display, DefaultScreen(display))); // Set the foreground color to white
    XFillRectangle(display, window, gc, 50, 50, 200, 100); // Draw a white rectangle
    XFlush(display);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <width> <height> <program_path>" << std::endl;
        return 1;
    }

    int screen_width = std::atoi(argv[1]);
    int screen_height = std::atoi(argv[2]);
    const char* programPath = argv[3];

    // Initialize Xlib thread support
    XInitThreads();

    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Could not open X display." << std::endl;
        return 1;
    }

    Window root_window = RootWindow(display, DefaultScreen(display));

    // Create a black window with no border
    Window window = XCreateSimpleWindow(display, root_window, 0, 0, screen_width, screen_height, 0, 0, 0);
    XMapWindow(display, window);

    // Request exclusive fullscreen mode (optional)
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;  // _NET_WM_STATE_ADD
    xev.xclient.data.l[1] = wm_state;
    XSendEvent(display, DefaultRootWindow(display), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);


    Window terminal = XCreateSimpleWindow(display, terminal, 0, 0, 400, 200, 0, 0, BlackPixel(display, DefaultScreen(display)));
    XMapWindow(display, terminal);

    // Start and display the specified program in an XTerm window
    startProgramInXTerm(display, terminal, programPath);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
    }

    XCloseDisplay(display);
    return 0;
}
