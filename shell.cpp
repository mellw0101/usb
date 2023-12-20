#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <unistd.h> // For usleep (to simulate progress)
#include <ctime>
#include <thread>
#include <fstream>
#include <map>
#include <chrono>
#include <SDL2/SDL.h>
#include <X11/cursorfont.h> // Include this header for cursor fonts
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <filesystem>
#include <X11/Xos.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <X11/extensions/Xrandr.h>
#include <unistd.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fcntl.h>
#include <cerrno>
#include <cstdio>
#include <pty.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdio>
#include <pty.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/wait.h>
#include <ncurses.h>


extern "C" {
#include <libavcodec/avcodec.h>
}




namespace fs = std::filesystem;
// Custom data structure to manage window creation order
class WindowManager {
public:
    WindowManager(Display* display) : display(display) {}

    // Create a new window and add it to the window stack
    Window createWindow(int x, int y, int width, int height) {
        Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), x, y, width, height, 0, 0, 0);
        windows.push_back(window);
        XMapWindow(display, window);
        XFlush(display);
        return window;
    }

    // Raise a window to the top of the stack
    void raiseWindow(Window window) {
        XRaiseWindow(display, window);
        XFlush(display);
    }

    // Lower a window to the bottom of the stack
    void lowerWindow(Window window) {
        XLowerWindow(display, window);
        XFlush(display);
    }

private:
    Display* display;
    std::vector<Window> windows;
};



// Global variable for the interval in milliseconds
int intervalMillis = 5000; // Default interval: 5 seconds
// Customize these values to match your shell and desired check interval
    std::string shellProcessName = "my_shell";  // Replace with your shell process name
    std::string shellCommand = "/path/to/your/shell";  // Replace with the full path to your shell executable
    int checkIntervalSeconds = 60;  // Adjust the interval (e.g., 60 seconds)

    //ensureShellRunning(shellProcessName, shellCommand, checkIntervalSeconds);

// Structure to store window information
struct WindowInfo {
    int x;
    int y;
};
// Map to store window IDs and their positions
std::map<Window, WindowInfo> windowMap;


bool createPtyPair(int& master_fd, int& slave_fd, char* slave_name, size_t slave_name_size) {
    // Open a new PTY master
    master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (master_fd == -1) {
        std::cerr << "Failed to open PTY master: " << strerror(errno) << std::endl;
        return false;
    }

    // Grant access to the slave
    if (grantpt(master_fd) == -1) {
        std::cerr << "Failed to grant access to PTY slave: " << strerror(errno) << std::endl;
        close(master_fd);
        return false;
    }

    // Unlock the slave
    if (unlockpt(master_fd) == -1) {
        std::cerr << "Failed to unlock PTY slave: " << strerror(errno) << std::endl;
        close(master_fd);
        return false;
    }

    // Get the name of the slave PTY
    if (ptsname_r(master_fd, slave_name, slave_name_size) != 0) {
        std::cerr << "Failed to get PTY slave name: " << strerror(errno) << std::endl;
        close(master_fd);
        return false;
    }

    // Open the slave PTY
    slave_fd = open(slave_name, O_RDWR);
    if (slave_fd == -1) {
        std::cerr << "Failed to open PTY slave: " << strerror(errno) << std::endl;
        close(master_fd);
        return false;
    }

    return true;
}
void animateWindowResize(Display* display, Window window, int targetWidth, int targetHeight, int animationDuration) {
    int screen = DefaultScreen(display);
    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);

    int currentWidth = attr.width;
    int currentHeight = attr.height;

    int steps = animationDuration / 50; // 50 milliseconds per step (adjust as needed)
    int widthStep = (targetWidth - currentWidth) / steps;
    int heightStep = (targetHeight - currentHeight) / steps;

    for (int i = 0; i < steps; i++) {
        currentWidth += widthStep;
        currentHeight += heightStep;

        XResizeWindow(display, window, currentWidth, currentHeight);
        XFlush(display);

        usleep(50000); // Sleep for 50 milliseconds
    }

    // Ensure the window reaches the exact target dimensions
    XResizeWindow(display, window, targetWidth, targetHeight);
    XFlush(display);
}
void PipeToShell() {
    // Open a shell process
    FILE* shellProcess = popen("/bin/bash", "w");
    if (!shellProcess) {
        std::cerr << "Error: Failed to open a shell process." << std::endl;
        return;
    }

    char inputBuffer[1024];
    char outputBuffer[1024];

    while (true) {
        // Read user input
        std::cout << "Enter a command (or 'exit' to quit): ";
        std::cin.getline(inputBuffer, sizeof(inputBuffer));

        // Check for the 'exit' command
        if (strcmp(inputBuffer, "exit") == 0) {
            break;
        }

        // Send the user input to the shell process
        std::fprintf(shellProcess, "%s\n", inputBuffer);
        std::fflush(shellProcess);

        // Read and display the shell's output
        while (std::fgets(outputBuffer, sizeof(outputBuffer), shellProcess)) {
            std::cout << "Shell Output: " << outputBuffer;
        }
    }

    // Close the shell process
    std::fclose(shellProcess);
}
// Function to create a basic terminal window
void createTerminalWindowXonly() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: X11 display initialization failed." << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // Create the main window
    Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                           BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, mainWindow);
    XFlush(display);

    // Open a shell process
    FILE* shellProcess = popen("/bin/bash", "w");
    if (!shellProcess) {
        std::cerr << "Error: Failed to open a shell process." << std::endl;
        XCloseDisplay(display);
        return;
    }

    // Set up input and output pipes for communication
    int x11InputPipe = ConnectionNumber(display);
    int shellOutputPipe = fileno(shellProcess);

    fd_set readSet;
    char buffer[1024];

    while (1) {
        FD_ZERO(&readSet);
        FD_SET(x11InputPipe, &readSet);
        FD_SET(shellOutputPipe, &readSet);

        int maxFd = std::max(x11InputPipe, shellOutputPipe) + 1;

        if (select(maxFd, &readSet, nullptr, nullptr, nullptr) < 0) {
            std::cerr << "Error: Select failed." << std::endl;
            break;
        }

        if (FD_ISSET(x11InputPipe, &readSet)) {
            int bytesRead = read(x11InputPipe, buffer, sizeof(buffer));
            if (bytesRead > 0) {
                write(shellOutputPipe, buffer, bytesRead);
            }
        }

        if (FD_ISSET(shellOutputPipe, &readSet)) {
            int bytesRead = read(shellOutputPipe, buffer, sizeof(buffer));
            if (bytesRead > 0) {
                write(x11InputPipe, buffer, bytesRead);
            }
        }
    }

    // Close the shell process and X11 display
    pclose(shellProcess);
    XCloseDisplay(display);
}
void createTerminalWindowXterm() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: X11 display initialization failed." << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // Create the main window
    Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                           BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, mainWindow);
    XFlush(display);

    // Launch xterm within the window
    std::string xtermCommand = "xterm -into " + std::to_string(mainWindow);
    system(xtermCommand.c_str());

    // Event loop to keep the window open
    XEvent event;
    bool running = true;

    while (running) {
        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            // Exit the program on any key press
            running = false;
        }
    }

    XDestroyWindow(display, mainWindow);
    XCloseDisplay(display);
}
// Function to create an X11 window and display a basic file tree
// Function to execute the 'tree' command and display its output in an X11 window
void displayFileTreeX11() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: X11 display initialization failed." << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // Create the main window
    Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                           BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, mainWindow);
    XFlush(display);

    XSelectInput(display, mainWindow, ExposureMask | KeyPressMask);

    XEvent event;

    // Execute the 'tree' command and capture its output
    FILE* treeCmd = popen("tree /path/to/your/directory", "r"); // Replace with your directory path
    if (!treeCmd) {
        std::cerr << "Error: Failed to execute the 'tree' command." << std::endl;
        return;
    }

    char buffer[1024];
    std::string treeOutput;

    while (fgets(buffer, sizeof(buffer), treeCmd) != nullptr) {
        treeOutput += buffer;
    }

    pclose(treeCmd);

    // Display the 'tree' command output on the window
    bool running = true;
    int yOffset = 20; // Initial y position for text

    while (running) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Display the 'tree' output on the window
            XClearWindow(display, mainWindow);

            const char* treeText = treeOutput.c_str();
            char* line = strtok(const_cast<char*>(treeText), "\n");

            while (line != nullptr) {
                XDrawString(display, mainWindow, DefaultGC(display, screen), 10, yOffset, line, strlen(line));
                yOffset += 20; // Move down for the next line
                line = strtok(nullptr, "\n");
            }
        } else if (event.type == KeyPress) {
            // Exit the program on any key press
            running = false;
        }
    }

    XDestroyWindow(display, mainWindow);
    XCloseDisplay(display);
}

// Function to display the filesystem as a tree
void displayFileSystemTree(const fs::path& path, int level = 0) {
    if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            for (int i = 0; i < level; ++i) {
                std::cout << "  "; // Indent based on the level
            }

            std::cout << "|-- " << entry.path().filename() << std::endl;

            if (fs::is_directory(entry)) {
                displayFileSystemTree(entry, level + 1); // Recursively display subdirectories
            }
        }
    }
}
// Function to check the framerate of an X11 window
void checkX11WindowFramerate(Display* display, Window window) {
    const int frameCount = 100; // Number of frames to capture
    struct timespec startTime, endTime;
    long frameDurations[frameCount];
    int frameIndex = 0;
    float averageFramerate = 0;

    for (int i = 0; i < frameCount; ++i) {
        clock_gettime(CLOCK_MONOTONIC, &startTime); // Get start time

        // Render something in the X11 window (replace with your rendering code)
        // You'll need to use Xlib or a similar library for rendering
        // Add your rendering code here...

        XFlush(display); // Ensure the rendering is complete

        // Get end time and calculate frame duration
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        long frameDuration = (endTime.tv_sec - startTime.tv_sec) * 1000 +
                             (endTime.tv_nsec - startTime.tv_nsec) / 1000000;

        frameDurations[frameIndex] = frameDuration;
        frameIndex = (frameIndex + 1) % frameCount;

        // Calculate the average framerate
        long totalFrameDuration = 0;
        for (int j = 0; j < frameCount; ++j) {
            totalFrameDuration += frameDurations[j];
        }
        averageFramerate = 1000.0f / (static_cast<float>(totalFrameDuration) / frameCount);

        std::cout << "Average Framerate: " << averageFramerate << " FPS" << std::endl;

        // Add a delay to maintain the desired framerate
        if (averageFramerate > 60.0f) {
            usleep(1000 * (1000 / 60)); // Cap at 60 FPS
        }
    }
}
// Function to check the framerate of an SDL window
void checkWindowFramerate(SDL_Window* window) {
    // Set up variables for framerate calculation
    const int frameCount = 100; // Number of frames to capture
    int startTime = 0;
    int endTime = 0;
    int frameTimes[frameCount];
    int frameTimeIndex = 0;
    float averageFramerate = 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: SDL initialization failed." << std::endl;
        return;
    }

    // Create an SDL renderer (assuming you already have a window)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Error: SDL renderer creation failed." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Main loop for capturing frames
    for (int i = 0; i < frameCount; ++i) {
        startTime = SDL_GetTicks(); // Get the start time of the frame

        // Render something in the window (replace with your rendering code)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color (black)
        SDL_RenderClear(renderer);
        // Add your rendering code here...

        SDL_RenderPresent(renderer); // Present the frame

        // Calculate the time it took to render this frame
        endTime = SDL_GetTicks();
        frameTimes[frameTimeIndex] = endTime - startTime;
        frameTimeIndex = (frameTimeIndex + 1) % frameCount;

        // Calculate the average framerate
        int totalFrameTime = 0;
        for (int j = 0; j < frameCount; ++j) {
            totalFrameTime += frameTimes[j];
        }
        float averageFramerate = 0.0f;
        if (frameCount > 0 && totalFrameTime > 0.0f) {
            averageFramerate = 1000.0f / (totalFrameTime / static_cast<float>(frameCount));
        } else {
            // Handle the case where frameCount or totalFrameTime is zero or negative
            // You can set a default or handle the error as needed.
        }

        std::cout << "Average Framerate: " << averageFramerate << " FPS" << std::endl;

        // Add a delay to maintain the desired framerate
        if (averageFramerate > 60.0f) {
            SDL_Delay(static_cast<Uint32>(1000.0f / 60.0f)); // Cap at 60 FPS
        }
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Function to enable the cursor for a window
void enableCursorForWindow(Display* display, Window window) {
    Cursor cursor = XCreateFontCursor(display, XC_watch);
    XDefineCursor(display, window, cursor);
    XFlush(display);
}
// Function to draw a simple shape
void drawSimpleShape() {
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Simple Graphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error: Unable to create SDL window." << std::endl;
        return;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error: Unable to create SDL renderer." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Set the drawing color (e.g., red)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_RenderFillRect(renderer, &rect);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for a few seconds before closing (adjust as needed)
    SDL_Delay(3000);

    // Clean up and close
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
// Function to check if the shell process is running
bool isShellRunning(const std::string& shellProcessName) {
    std::string command = "pgrep " + shellProcessName;
    return system(command.c_str()) == 0;
}

// Function to start the shell process
void startShell(const std::string& shellCommand) {
    system(shellCommand.c_str());
}

// Function to ensure the shell is always running
void ensureShellRunning(const std::string& shellProcessName, const std::string& shellCommand, int checkIntervalSeconds) {
    while (true) {
        if (!isShellRunning(shellProcessName)) {
            std::cout << "Shell is not running. Starting it..." << std::endl;
            startShell(shellCommand);
        }

        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::seconds(checkIntervalSeconds));
    }
}
// Function to update the window map with the current window positions
void updateWindowMap(Display* display) {
    Window root = RootWindow(display, DefaultScreen(display));
    Window parent;
    Window* children;
    unsigned int numChildren;

    if (XQueryTree(display, root, &root, &parent, &children, &numChildren)) {
        for (unsigned int i = 0; i < numChildren; ++i) {
            XWindowAttributes attr;
            XGetWindowAttributes(display, children[i], &attr);
            windowMap[children[i]] = {attr.x, attr.y};
        }
        XFree(children);
    }
}
// Function to print the window map
void printWindowMap() {
    for (const auto& pair : windowMap) {
        std::cout << "Window ID: " << pair.first << ", X: " << pair.second.x << ", Y: " << pair.second.y << std::endl;
    }
}
// Function to periodically check window positions based on the global interval
void checkWindowPositions() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: Unable to open X display." << std::endl;
        return;
    }

    while (1) {
        updateWindowMap(display);
        printWindowMap();

        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMillis));
    }

    XCloseDisplay(display);
}
// Function to move a window to a new position
void moveWindow(Display* display, Window window, int newX, int newY) {
    XMoveWindow(display, window, newX, newY);
    XFlush(display);
}
// Function to create a window with specified dimensions and title
void createWindow(Display* display, const std::string& title, int width, int height) {
    Window rootWindow = RootWindow(display, DefaultScreen(display));
    Window newWindow = XCreateSimpleWindow(display, rootWindow, 100, 100, width, height, 2,
                                           BlackPixel(display, DefaultScreen(display)), WhitePixel(display, DefaultScreen(display)));

    XStoreName(display, newWindow, title.c_str());
    XMapWindow(display, newWindow);
    XFlush(display);

    // Event loop to keep the new window open
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        // Handle events here
    }
}
// Function to read a file and create a window based on its content
void createWindowFromFile(Display* display, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }

    std::string title, widthStr, heightStr;
    if (std::getline(file, title) && std::getline(file, widthStr) && std::getline(file, heightStr)) {
        int width = std::stoi(widthStr);
        int height = std::stoi(heightStr);
        createWindow(display, title, width, height);
    } else {
        std::cerr << "Error: File format is invalid." << std::endl;
    }
}
void openNewWindow(Display* display) {
    // Create a new X11 window
    Window newWindow = XCreateSimpleWindow(display, DefaultRootWindow(display), 100, 100, 400, 300, 2,
                                           BlackPixel(display, DefaultScreen(display)), WhitePixel(display, DefaultScreen(display)));

    // Map and display the new window
    XMapWindow(display, newWindow);
    XFlush(display);

    // Event loop to keep the new window open
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        // Handle events here
    }
}
// Function to close all open windows except the specified shell window
void closeAllWindowsExcept(Display* display, Window shell) {
    Window root;
    Window parent;
    Window* children;
    unsigned int numChildren;

    XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &numChildren);

    for (unsigned int i = 0; i < numChildren; i++) {
        Window child = children[i];
        if (child != shell) {
            XKillClient(display, child); // Forcefully terminate the window
        }
    }

    if (children) {
        XFree(children);
    }
}
// Function to kill a specific window by its XID
void HardKillWindow(Display* display, Window window) {
    XKillClient(display, window); // Forcefully terminate the window
    XFlush(display);
}
// Function to wait for a specified time (in seconds)
void waitForTime(int seconds) {
    time_t startTime = time(nullptr);
    while (time(nullptr) - startTime < seconds) {
        // Wait
    }
}
// Function to attempt to close a window if it doesn't respond within a timeout
void closeWindowIfNotResponding(Display* display, Window window, int timeoutSeconds) {
    XWindowAttributes windowAttributes;
    Status status = XGetWindowAttributes(display, window, &windowAttributes);
    if (status != 0 && windowAttributes.map_state == IsViewable) {
        // The window is mapped and visible

        // Wait for the specified timeout
        waitForTime(timeoutSeconds);

        // Check if the window is still responding
        XEvent event;
        XSync(display, False);
        if (XCheckTypedWindowEvent(display, window, ClientMessage, &event)) {
            // The window responded within the timeout
            std::cout << "Window responded within the timeout." << std::endl;
        } else {
            // The window did not respond within the timeout, attempt to close it
            std::cout << "Window did not respond within the timeout. Attempting to close." << std::endl;
            HardKillWindow(display, window);
        }
    } else {
        // The window is not mapped or not visible
        std::cout << "Window is not visible." << std::endl;
    }
}
void closeWindow(Display* display, Window window) {
    XEvent ev;
    Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(display, window, &wmDelete, 1);

    // Send a ClientMessage to request window closure
    ev.type = ClientMessage;
    ev.xclient.window = window;
    ev.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", False);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = wmDelete;
    ev.xclient.data.l[1] = CurrentTime;

    XSendEvent(display, window, False, NoEventMask, &ev);
    XFlush(display);
}
void closeAllWindowsExcept(Display* display, Window background, Window shell) {
    Window root;
    Window parent;
    Window* children;
    unsigned int numChildren;

    XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &numChildren);

    for (unsigned int i = 0; i < numChildren; i++) {
        Window child = children[i];
        if (child != background && child != shell) {
            closeWindow(display, child);
        }
    }

    if (children) {
        XFree(children);
    }
}
// Function to raise the specified window to the top of the window stack
void elevateWindow(Display* display, Window window) {
    XRaiseWindow(display, window);
    XFlush(display);
}
// Function to wait for a specified time (in seconds)
void waitsec(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
// Function to create a simple message window
Window createMessageWindow(Display* display, int x, int y, int width, int height, const char* message) {
    int screen = DefaultScreen(display);
    Window parent = RootWindow(display, screen);
    Window messageWindow = XCreateSimpleWindow(display, parent, x, y, width, height, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XMapWindow(display, messageWindow);
    XFlush(display);

    XStoreName(display, messageWindow, message);

    return messageWindow;
}
void displayProgressBar(int progress, int total, int barWidth = 50) {
    float percentage = static_cast<float>(progress) / total;
    int numChars = static_cast<int>(percentage * barWidth);

    std::cout << "[";

    for (int i = 0; i < barWidth; ++i) {
        if (i < numChars) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }

    std::cout << "] " << std::fixed << std::setprecision(1) << (percentage * 100) << "%\r";
    std::cout.flush();
}
void drawProgressBar(Display* display, Window window, int progress, int total, int barWidth) {
    XGCValues gcValues;
    GC gc = XCreateGC(display, window, 0, &gcValues);

    // Calculate the width of the filled bar
    int filledWidth = (progress * barWidth) / total;

    // Clear the window
    XClearWindow(display, window);

    // Set the color for the filled portion of the progress bar
    XSetForeground(display, gc, 0x00FF00); // Green color

    // Draw the filled portion of the progress bar
    XFillRectangle(display, window, gc, 0, 0, filledWidth, 20);

    // Set the color for the remaining portion of the progress bar
    XSetForeground(display, gc, 0xFF0000); // Red color

    // Draw the remaining portion of the progress bar
    XFillRectangle(display, window, gc, filledWidth, 0, barWidth - filledWidth, 20);

    // Free the graphics context
    XFreeGC(display, gc);

    // Flush the display to update the window
    XFlush(display);
 //int total = 100;
  //  int barWidth = 400;

    //for (int progress = 0; progress <= total; ++progress) {
      //  drawProgressBar(display, window, progress, total, barWidth);
       // usleep(100000); // Sleep for 100 milliseconds (0.1 seconds) to simulate progress
    //}

}
void createSlimBezel(Display* display, Window window) {
    // Set the border width (adjust as needed)
    unsigned int borderWidth = 2;
    XSetWindowBorderWidth(display, window, borderWidth);

    // Set the border color (adjust as needed)
    XSetWindowBorder(display, window, BlackPixel(display, DefaultScreen(display)));
    XSetWindowBorder(display, window, WhitePixel(display, DefaultScreen(display)));

    // Ensure the changes are applied
    XFlush(display);
}
// Function to create a virtual display using Xephyr
bool createVirtualDisplay(int width, int height, const std::string& displayNumber) {
    std::ostringstream command;
    command << "Xephyr -screen " << width << "x" << height << " :" << displayNumber;

    int result = system(command.str().c_str());

    if (result != 0) {
        std::cerr << "Error: Unable to create a virtual display." << std::endl;
        return false;
    }

    return true;
}
bool executeRemoteCommand(const std::string& remoteHost, const std::string& remoteUser, const std::string& sshKeyPath, const std::string& command) {
    std::string sshCommand = "ssh -i " + sshKeyPath + " " + remoteUser + "@" + remoteHost + " \"" + command + "\"";

    int result = system(sshCommand.c_str());

    if (result != 0) {
        std::cerr << "Error: Failed to execute remote command." << std::endl;
        return false;
    }

    return true;
}
// Function to start a program on a virtual display
bool runOnVirtualDisplay(const std::string& displayNumber, const std::string& programPath) {
    std::ostringstream command;
    command << "DISPLAY=:" << displayNumber << " " << programPath;

    int result = system(command.str().c_str());

    if (result != 0) {
        std::cerr << "Error: Unable to run the program on the virtual display." << std::endl;
        return false;
    }

    return true;
}
bool startVirtualXServer(const std::string& displayNumber) {
    std::string command = "Xephyr :"+displayNumber+" -ac -screen 1024x768 &";  // Use Xephyr, change resolution as needed

    // Alternatively, for Xvfb:
    // std::string command = "Xvfb :"+displayNumber+" -screen 0 1024x768x24 &";

    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error: Unable to start virtual X server." << std::endl;
        return false;
    }

    return true;
}
// Function to set the _NET_WM_STATE_FULLSCREEN property
void setFullscreen(Display* display, Window window) {
    Atom net_wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom net_wm_state_fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    if (net_wm_state != None && net_wm_state_fullscreen != None) {
        XChangeProperty(display, window, net_wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&net_wm_state_fullscreen, 1);
    }
}

// Function to make a window non-interactive
void makeNonInteractive(Display* display, Window window) {
    XWMHints hints;
    hints.flags = InputHint;
    hints.input = False;
    XSetWMHints(display, window, &hints);
    XFlush(display);
}

// Function to set the _NET_WM_STATE_ABOVE property to keep a window on top
void setAbove(Display* display, Window window) {
    Atom net_wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom net_wm_state_above = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

    if (net_wm_state != None && net_wm_state_above != None) {
        XChangeProperty(display, window, net_wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&net_wm_state_above, 1);
    }
}
void setBelow(Display* display, Window window) {
    Atom net_wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom net_wm_state_below = XInternAtom(display, "_NET_WM_STATE_BELOW", False);

    if (net_wm_state != None && net_wm_state_below != None) {
        XChangeProperty(display, window, net_wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&net_wm_state_below, 1);
    }
}
Window createBackground(Display* display, Window rootWindow) {
    int screen = DefaultScreen(display);
    Window background = XCreateSimpleWindow(display, rootWindow, 0, 0,
                                            DisplayWidth(display, screen),
                                            DisplayHeight(display, screen),
                                            0, BlackPixel(display, screen),
                                            WhitePixel(display, screen));

    // Make the background window exclusive fullscreen
    setFullscreen(display, background);

    // Make the background window non-interactive
    makeNonInteractive(display, background);

    // Set the background window as an "above" window to keep it below
    setBelow(display, background);

    XMapWindow(display, background);
    XFlush(display);

    return background;
}
Window shellWindow;
int main_x(int argc, char* argv[]);
void runShellInXTerm(Display* display, Window window, const std::string& programPath) {
    // Run your shell in the xterm within the shell window
    std::string command = "xterm -into " + std::to_string(static_cast<unsigned long>(shellWindow)) + " -e " + programPath;
    int shellExitStatus = system(command.c_str());

    if (shellExitStatus != 0) {
        std::cerr << "Error: Failed to run the shell." << std::endl;
        exit(1);
    }
}
std::string captureShellOutput(const char* command);
int main_x(int argc, char* argv[]) {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        return 1;
    }

    std::string programPath = "/home/SERVER/test";              // Default shell program path

    // Default terminal window size
    int terminalWidth = 800;
    int terminalHeight = 600;

    // Process command-line arguments
    if (argc >= 2) {
        programPath = argv[1]; // Use the first argument as the program path
    }
    if (argc >= 4) {
        terminalWidth = std::atoi(argv[2]); // Use the second argument as the width
        terminalHeight = std::atoi(argv[3]); // Use the third argument as the height
    }

    // Get the root window
    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // Create the background window
    Window background = createBackground(display, rootWindow);

    // Create another window for your terminal
    Window shellWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, terminalWidth, terminalHeight, 0, 0, BlackPixel(display, screen));
    XMapWindow(display, shellWindow);
    setAbove(display, shellWindow);
    XFlush(display);

    // Run your shell in the xterm within the shell window
    runShellInXTerm(display, shellWindow, programPath);

    // Event loop to keep the windows open and allow movement
    XEvent event;
    bool isMoving = false;
    int xOffset = 0, yOffset = 0;

    while (1) {
        XNextEvent(display, &event);

        // Handle events for your terminal window (shellWindow) here...

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
    // Check for user input to execute and capture output
        if (event.type == KeyPress) {
            KeySym key;
            char text[255];
            XLookupString(&event.xkey, text, sizeof(text), &key, 0);

            // Assuming Enter key triggers command execution
            if (key == XK_Return) {
                // Example command to run
                const char* commandToRun = "ls -l";

                // Capture the output
                std::string output = captureShellOutput(commandToRun);

                // Display the captured output
                std::cout << "Command Output:\n" << output << std::endl;
            }
        }
    }


    XCloseDisplay(display);
    return 0;
}

void executeShellInPty(int slave_fd) {
    // Redirect stdin, stdout, and stderr to the slave PTY
    dup2(slave_fd, 0); // STDIN
    dup2(slave_fd, 1); // STDOUT
    dup2(slave_fd, 2); // STDERR

    // Close the original slave_fd as it's no longer needed
    close(slave_fd);
    std::string shell = "/home/SERVER/test";
    // Execute your shell or command (replace "/bin/bash" with your desired command)
    execl(shell.c_str(), shell.c_str(), NULL);

    // If execl fails, handle the error
    perror("execl");
    exit(EXIT_FAILURE);
}
void renderTerminal(GLFWwindow* window) {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // Create an OpenGL texture to render the terminal content
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Handle OpenGL rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the terminal content
        // Read the terminal content using ncurses and update the OpenGL texture

        // Example: Render a simple message
        mvprintw(10, 10, "Hello, OpenGL Terminal!");

        // Refresh the ncurses screen
        refresh();

        // Update the OpenGL texture with the terminal content
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // Render the OpenGL texture on a quad
        // You need to set up the rendering of the texture on a quad here

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    glDeleteTextures(1, &texture);
    endwin();
}

// Function to capture text output from a shell command
std::string captureShellOutput(const char* command) {
    std::string result;
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        return "Error: Unable to run the shell command.";
    }

    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }

    pclose(pipe);
    return result;
}
int main_gl() {
    if (!glfwInit()) {
        std::cerr << "Error: GLFW initialization failed." << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Terminal", NULL, NULL);
    if (!window) {
        std::cerr << "Error: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: GLEW initialization failed." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Call the function to render the terminal content
    renderTerminal(window);

    // Clean up and terminate GLFW
    glfwTerminate();

    return 0;
}

int main(){
   startVirtualXServer("3");
   return 0;
}
