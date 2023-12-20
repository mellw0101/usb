#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <cstddef>
#include <iostream>
#include <X11/Xlib.h>      // For basic Xlib functions and types
#include <X11/Xatom.h>     // For X atoms (e.g., _NET_WM_STATE)
#include <X11/Xutil.h>     // For XSelectInput
#include <X11/Xos.h>       // For XInitThreads
#include <X11/Xlibint.h>   // For XInternAtom
#include <X11/extensions/Xrandr.h> // If you plan to work with screen resolution

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
    XFillRectangle(display, window, gc, 0, 0, 800, 400); // Draw a white rectangle
    XFlush(display);
}

int main(int argc, char* argv[]) {
     if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <program_path>" << std::endl;
        return 1;
    }
    std::string programPath = argv[1];

    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Error: Could not open X display." << std::endl;
        return 1;
    }

    // Initialize Xlib thread support
    XInitThreads();

    // Get the default screen and screen dimensions
    int screen = DefaultScreen(display);
    int width = 800;
    int height = 600;

    // Create another window for your shell
    Window shellWindow = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, 800, 600, 0, 0, BlackPixel(display, screen));
    XMapWindow(display, shellWindow);
    XFlush(display);
    startProgramInXTerm(display, shellWindow, programPath.c_str());
    XMapWindow(display, shellWindow);
    XFlush(display);

    // Start your shell in the new XTerm window
    // Change the program path as needed

    // Event loop to keep the windows open and allow movement
    XEvent event;
    bool isMoving = false;
    int xOffset = 0, yOffset = 0;

    while (1) {
        XNextEvent(display, &event);

        if (event.type == ButtonPress && event.xbutton.button == 1) {
            // Left mouse button press; start moving the shell window
            isMoving = true;
            xOffset = event.xbutton.x;
            yOffset = event.xbutton.y;
        } else if (event.type == MotionNotify && isMoving) {
            // Mouse motion while moving; reposition the shell window
            XMoveWindow(display, shellWindow, event.xmotion.x - xOffset, event.xmotion.y - yOffset);
        } else if (event.type == ButtonRelease && event.xbutton.button == 1) {
            // Left mouse button release; stop moving
            isMoving = false;
        }
    }

    XCloseDisplay(display);
    return 0;
}
