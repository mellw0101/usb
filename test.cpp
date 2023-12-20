#include <X11/X.h>
#include <array>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <curses.h>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/mount.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>
#include <ext2fs/ext2fs.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <thread>
#include <sstream>
#include <chrono>
#include <openssl/sha.h>
#include <iomanip>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <zlib.h>
#include <regex>
#include <readline/readline.h>
#include <readline/history.h>
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
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Text.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <string>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <string>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <string>
#include <vector>




std::vector<std::string> commandHistory;
int historyIndex = -1;

namespace fs = std::filesystem;

namespace good_funcs {
    void error_message(const std::string& program, const std::string& message) {
        std::cerr << program + ": ERROR: " << message << " (press enter to continue)";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    void input_to_file(const std::string& input, const std::string& directory, const std::string& FILEname) {
        std::string full_path = directory + "/" + FILEname;
        if (fs::exists(directory) && fs::is_directory(directory)) {
            std::cout << "input_to_file: Directory: '" << directory << "' is a directory\n";
            if (fs::exists(full_path) && fs::is_directory(full_path)) {
                error_message("input_to_file", "target file '" + FILEname + "' is a directory");
                return;
            }
            if (fs::exists(full_path)) {
                std::cout << "input_to_file: target file '" << full_path << "' exists\n";
                std::ofstream openFILE;
                openFILE.open(full_path, std::ios::app);
                if (openFILE.is_open()) {
                    openFILE << input << std::endl;
                    openFILE.close();
                    std::cout << "input_to_file: successfully appended '" << input << "' to '" << full_path << "'\n";
                } else {
                    error_message("input_to_file", "can't open '" + FILEname + "'");
                    return;
                }
            } else {
                error_message("input_to_file", "target file '" + full_path + "' does not exist");
                return;
            }
        } else if (fs::exists(directory)) {
            error_message("input_to_file", "'" + directory + "' is not a directory; it is a file");
            return;
        } else {
            error_message("input_to_file", "Directory '" + directory + "' does not exist");
            return;
        }
    }
    void create_file(const std::string& full_PATH_to_file) {
        if (fs::exists(full_PATH_to_file)) {
            if (fs::is_directory(full_PATH_to_file)) {
                error_message("create_file", "target file '" + full_PATH_to_file + "' is a directory");
                return;
            } else {
                error_message("create_file", "target file '" + full_PATH_to_file + "' already exists");
                return;
            }
        } else {
            std::ofstream outFile;
            outFile.open(full_PATH_to_file, std::ios::out | std::ios::trunc);
            if (!outFile.is_open()) {
                error_message("create_file", "Failed to create a new file at target '" + full_PATH_to_file + "'");
                return;
            } else {
                outFile.close();
                std::cout << "create_file: created an empty file at '" << full_PATH_to_file << "'\n";
                return;
            }
        }
    }
    void export_from_file(const std::string& FILEname, std::string& evar) {
        if (fs::exists(FILEname)) {  // if target file exists
            if (!fs::is_directory(FILEname)) {  // if target file is not a directory
                if (!fs::is_empty(FILEname)) {  // if target file is not empty
                    std::ifstream inFile;
                    inFile.open(FILEname);
                    if (inFile.is_open()) {  // if function was able to open the file
                        std::cout << "export_from_file: successfully opened target file '" + FILEname + "'\n";
                        std::string var;
                        std::stringstream buffer;
                        while (getline(inFile, var)) {
                            buffer << var << '\n';
                        }
                        evar = buffer.str();
                        if (!evar.empty()) {
                            std::cout << "export_from_file: successfully read file '" + FILEname + "'\n";
                        }
                        inFile.close();
                    } else {  // if function was unable to open the file
                        error_message("export_from_file", "can't open '" + FILEname + "'");
                    }
                } else {  // if target file is empty
                    error_message("export_from_file", "target file '" + FILEname + "' is empty");
                }
            } else {  // if target file is a directory
                error_message("export_from_file", "target file '" + FILEname + "' is a directory");
            }
        } else {  // if target file does not exist
            error_message("export_from_file", "target file '" + FILEname + "' does not exist");
        }
    }
    bool search_string(std::string& targetstring, const std::string& wordToFind) {
        // Create a regular expression pattern to match the whole word
        std::regex wordPattern("\\b" + wordToFind + "\\b");

        // Search for the whole word in the string
        std::smatch match;
        if (std::regex_search(targetstring, match, wordPattern)) {
            std::cout << "search_string: Word '" << wordToFind << "' found at position " << match.position() << std::endl;
            return true;
        } else {
            std::cout << "search_string: Word '" << wordToFind << "' not found" << std::endl;
            return false;
        }
    }
    void addToHistory(const std::string& command) {
        commandHistory.push_back(command);
        historyIndex = commandHistory.size() - 1;
    }
    void displayHistory() {
        std::cout << "Command History:" << std::endl;
        for (const std::string& cmd : commandHistory) {
            std::cout << cmd << std::endl;
        }
    }
    void executeStoredCommand(const std::string& command);
    void executeCommand(const std::vector<std::string>& args) {
        if (args.empty()) {
            return;  // No command to execute
        }

        addToHistory(args[0]);  // Add the command to history

        // Check for custom commands
        if (args[0] == "history") {
            displayHistory();
        } else if (args[0] == "!!") {
            if (historyIndex >= 0 && historyIndex < static_cast<int>(commandHistory.size())) {
                executeStoredCommand(commandHistory[historyIndex]);
            }
        } else if (args[0][0] == '!' && args[0].size() > 1) {
            int commandNumber = std::stoi(args[0].substr(1));
            if (commandNumber >= 1 && commandNumber <= static_cast<int>(commandHistory.size())) {
                executeStoredCommand(commandHistory[commandNumber - 1]);
            }
        } else if (args[0] == "create_file") {
            if (args.size() == 2) {
                create_file(args[1]);
            } else {
                std::cout << "Usage: create_file <filename>" << std::endl;
            }
        } else if (args[0] == "export_from_file") {
            if (args.size() == 2) {
                std::string content;
                export_from_file(args[1], content);
                std::cout << "File content: " << content << std::endl;
            } else {
                std::cout << "Usage: export_from_file <filename>" << std::endl;
            }
        } else if (args[0] == "search_string") {
            if (args.size() == 3) {
                std::string targetstring = args[1];
                std::string wordToFind = args[2];
                search_string(targetstring, wordToFind);
            } else {
                std::cout << "Usage: search_string <targetstring> <wordToFind>" << std::endl;
            }
        } else {
            // Convert the vector of arguments to an array of C-style strings
            std::vector<char*> argv;
            for (const std::string& arg : args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);  // Null-terminate the array

            // Fork a child process
            pid_t pid = fork();

            if (pid == -1) {
                // Forking failed
                perror("fork");
            } else if (pid == 0) {
                // This is the child process
                // Execute the command
                if (execvp(argv[0], argv.data()) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else {
                // This is the parent process
                // Wait for the child to finish
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid");
                }
            }
        }
    }
    void executeStoredCommand(const std::string& command) {
        // Implement the execution of stored commands here
        std::vector<std::string> storedArgs;
        std::istringstream iss(command);
        std::string token;
        while (iss >> token) {
            storedArgs.push_back(token);
        }

        executeCommand(storedArgs);
    }

}
namespace test_funcs {
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
        FILE* shellProcess = popen("/home/SERVER/test", "w");
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
    void createX11WindowWithPTY() {
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

        // Create a pseudo-terminal (PTY)
        int master, slave;
        char buffer[1024];

        if (openpty(&master, &slave, nullptr, nullptr, nullptr) == -1) {
            std::cerr << "Error: Failed to create a pseudo-terminal." << std::endl;
            XCloseDisplay(display);
            return;
        }

        // Open a shell process in the slave end of the PTY
        pid_t childPid = fork();

        if (childPid == -1) {
            std::cerr << "Error: Fork failed." << std::endl;
            XCloseDisplay(display);
            return;
        }

        if (childPid == 0) { // Child process
            close(master); // Close master in the child process
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            dup2(slave, STDIN_FILENO);
            dup2(slave, STDOUT_FILENO);
            dup2(slave, STDERR_FILENO);

            // Execute a shell or your desired command
            execl("/bin/bash", "/bin/bash", nullptr);
        } else { // Parent process
            close(slave); // Close slave in the parent process

            while (1) {
                fd_set readSet;
                FD_ZERO(&readSet);
                FD_SET(master, &readSet);

                if (select(master + 1, &readSet, nullptr, nullptr, nullptr) < 0) {
                    std::cerr << "Error: Select failed." << std::endl;
                    break;
                }

                if (FD_ISSET(master, &readSet)) {
                    int bytesRead = read(master, buffer, sizeof(buffer));
                    if (bytesRead > 0) {
                        // Process and display the data received from the child process
                        write(STDOUT_FILENO, buffer, bytesRead);
                    }
                }
            }

            // Close the master end of the PTY and wait for the child process to exit
            close(master);
            int status;
            waitpid(childPid, &status, 0);
        }

        // Close the X11 display
        XCloseDisplay(display);
    }
    int pyy() {
        int master, slave;
        char buffer[4096];

        if (openpty(&master, &slave, nullptr, nullptr, nullptr) == -1) {
            perror("openpty");
            return 1;
        }

        pid_t childPid = fork();

        if (childPid == -1) {
            perror("fork");
            return 1;
        }

        if (childPid == 0) { // Child process
            close(master); // Close master in the child process
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            dup2(slave, STDIN_FILENO);
            dup2(slave, STDOUT_FILENO);
            dup2(slave, STDERR_FILENO);

            // Execute a shell or your desired command
            execl("/bin/bash", "/bin/bash", nullptr);
        } else { // Parent process
            close(slave); // Close slave in the parent process

            while (true) {
                ssize_t bytesRead = read(master, buffer, sizeof(buffer));

                if (bytesRead <= 0) {
                    break; // Exit when there's no more data
                }

                // Process and display the data received from the child process
                write(STDOUT_FILENO, buffer, bytesRead);
            }
        }

        return 0;
    }
    void drawRectangle(Display* display, Window window, int x, int y, int width, int height, unsigned long color) {
        XGCValues values;
        GC gc;

        // Create a graphics context
        gc = XCreateGC(display, window, 0, &values);

        // Set the foreground color
        XSetForeground(display, gc, color);

        // Draw the rectangle
        XFillRectangle(display, window, gc, x, y, width, height);

        // Free the graphics context
        XFreeGC(display, gc);
        XFlush(display);
    }
    void renderText(Display* display, Window window, int x, int y, const char* text, const char* fontName, int fontSize) {
        XFontStruct* fontInfo;
        XGCValues values;
        GC gc;

        // Load the font
        fontInfo = XLoadQueryFont(display, fontName);

        if (!fontInfo) {
            std::cerr << "Error: Font not found." << std::endl;
            return;
        }

        // Create a graphics context
        values.font = fontInfo->fid;
        gc = XCreateGC(display, window, GCFont, &values);

        // Set the font size
        XSetFont(display, gc, fontInfo->fid);

        // Set the drawing color (you can customize the color)
        XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));

        // Draw the text at the specified position
        XDrawString(display, window, gc, x, y, text, strlen(text));

        // Free the font and graphics context
        XFreeFontInfo(nullptr, fontInfo, 1);
        XFreeGC(display, gc);
        XFlush(display);
    }
    void renderText2(Display* display, Window window, int x, int y, const char* text, const char* fontName, int fontSize) {
        XFontStruct* fontInfo;
        XGCValues values;
        GC gc;

        // Load the font
        fontInfo = XLoadQueryFont(display, fontName);

        if (!fontInfo) {
            std::cerr << "Error: Font not found." << std::endl;
            return;
        }

        // Create a graphics context
        values.font = fontInfo->fid;
        gc = XCreateGC(display, window, GCFont, &values);

        // Set the font size
        XSetFont(display, gc, fontInfo->fid);

        // Set the drawing color to white
        XSetForeground(display, gc, WhitePixel(display, DefaultScreen(display)));

        // Draw the text at the specified position
        XDrawString(display, window, gc, x, y, text, strlen(text));

        // Free the font and graphics context
        XFreeFontInfo(nullptr, fontInfo, 1);
        XFreeGC(display, gc);
        XFlush(display);
    }
    void renderBlackBackground(Display* display, Window window) {
        XGCValues values;
        GC gc;

        // Create a graphics context
        gc = XCreateGC(display, window, 0, &values);

        // Set the background color to black
        XSetBackground(display, gc, BlackPixel(display, DefaultScreen(display)));

        // Fill the entire window with black
        XFillRectangle(display, window, gc, 0, 0, 800, 600);

        // Free the graphics context
        XFreeGC(display, gc);
        XFlush(display);
    }
    void createTextInputWindowsimple(Widget topLevel) {
        Widget form, label, textInput;

        // Create a form widget as the top-level container
        form = XtVaCreateManagedWidget("form", formWidgetClass, topLevel, nullptr);

        // Create a label widget for the prompt
        label = XtVaCreateManagedWidget("label", labelWidgetClass, form,
                                    XtNlabel, "Enter Text:",
                                    nullptr);

        // Create a text input widget
        textInput = XtVaCreateManagedWidget("textInput", textWidgetClass, form, nullptr);

        // Set the width of the text input widget (adjust as needed)
        XtVaSetValues(textInput, XtNwidth, 200, nullptr);
    }
    void createAndRenderTextWindow(const std::string& text) {
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Error: X11 display initialization failed." << std::endl;
            return;
        }

        int screen = DefaultScreen(display);
        Window rootWindow = RootWindow(display, screen);

        // Create the main window with a black background
        Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                                WhitePixel(display, screen), BlackPixel(display, screen));
        XMapWindow(display, mainWindow);
        XFlush(display);

        // Create a graphics context
        XGCValues values;
        GC gc = XCreateGC(display, mainWindow, 0, &values);

        // Set the font and font color (white)
        XFontStruct* fontInfo = XLoadQueryFont(display, "fixed");
        XSetFont(display, gc, fontInfo->fid);
        XSetForeground(display, gc, WhitePixel(display, screen));

        // Set the text position
        int x = 100;
        int y = 100;

        // Render the text
        XDrawString(display, mainWindow, gc, x, y, text.c_str(), text.length());
        XFlush(display);

        // Event loop
        XEvent event;
        while (1) {
            XNextEvent(display, &event);
            // Handle events or break the loop when done
        }

        // Close the X11 display (not reached in this example)
        XCloseDisplay(display);
    }
    void sendTextToWindow(Window window, const std::string& text) {
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Error: X11 display initialization failed." << std::endl;
            return;
        }

        XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

        XEvent event;
        KeySym keySym;
        KeyCode keyCode;

        for (const char& c : text) {
            keySym = XStringToKeysym(&c);
            if (keySym == NoSymbol) {
                // Character not found in X11 KeySym, try a more general approach
                if (c >= 32 && c <= 126) {
                    // Printable ASCII characters
                    keySym = XK_space + (c - ' ');
                } else {
                    // Non-printable characters (e.g., special keys)
                    continue;
                }
            }

            keyCode = XKeysymToKeycode(display, keySym);

            // Generate KeyPress event
            event.type = KeyPress;
            event.xkey.display = display;
            event.xkey.window = window;
            event.xkey.root = RootWindow(display, DefaultScreen(display));
            event.xkey.subwindow = None;
            event.xkey.time = CurrentTime;
            event.xkey.x = 1;
            event.xkey.y = 1;
            event.xkey.x_root = 1;
            event.xkey.y_root = 1;
            event.xkey.state = 0;
            event.xkey.keycode = keyCode;

            XSendEvent(display, window, True, KeyPressMask, &event);
            XFlush(display);

            usleep(10000); // Sleep for a short duration (microseconds) between KeyPress and KeyRelease events

            // Generate KeyRelease event
            event.type = KeyRelease;
            XSendEvent(display, window, True, KeyReleaseMask, &event);
            XFlush(display);

            usleep(10000); // Sleep again between characters (adjust as needed)
        }

        XCloseDisplay(display);
    }
    void displayCodeLinesWithRectangle(const std::vector<std::string>& codeLines) {
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Error: X11 display initialization failed." << std::endl;
            return;
        }

        int screen = DefaultScreen(display);
        Window rootWindow = RootWindow(display, screen);

        // Create the main window
        Window mainWindow = XCreateSimpleWindow(display, rootWindow, 500, 300, 800, 600, 0, 0, 0);
        XMapWindow(display, mainWindow);
        XFlush(display);

        // Create a graphics context
        XGCValues values;
        GC gc = XCreateGC(display, rootWindow, 0, &values);

        // Set the font and font color (white)
        XFontStruct* fontInfo = XLoadQueryFont(display, "fixed");
        XSetFont(display, gc, fontInfo->fid);
        XSetForeground(display, gc, 0);

        // Set the text position
        int x = 100;
        int y = 100;

        // Display code lines
        for (const std::string& line : codeLines) {
            XDrawString(display, mainWindow, gc, x, y, line.c_str(), line.length());
            y += fontInfo->ascent + fontInfo->descent; // Move to the next line
        }

        XFlush(display);

        // Event loop
        XEvent event;
        while (1) {
            XNextEvent(display, &event);
            // Handle events or break the loop when done
        }

        // Close the X11 display (not reached in this example)
        XCloseDisplay(display);
    }
    void runShellInXTerm(Display* display, Window window, const std::string& programPath) {
        // Run your shell in the xterm within the shell window
        std::string command = "xterm -into " + std::to_string(static_cast<unsigned long>(window)) + " -e " + programPath;
        int shellExitStatus = system(command.c_str());

        if (shellExitStatus != 0) {
            std::cerr << "Error: Failed to run the shell." << std::endl;
            exit(1);
        }
    }
    void RunPTY(const std::string& command, const std::string& input, std::string& output) {
        int master, slave;
        char buffer[4096];

        // Open a new PTY
        if (openpty(&master, &slave, NULL, NULL, NULL) == -1) {
            perror("openpty");
            return;
        }

        // Fork a child process
        pid_t child = fork();

        if (child == -1) {
            perror("fork");
            return;
        } else if (child == 0) {
            // Child process: Replace standard input/output/error with PTY
            close(master);

            // Redirect the slave side of PTY to the child's standard input/output/error
            dup2(slave, STDIN_FILENO);
            dup2(slave, STDOUT_FILENO);
            dup2(slave, STDERR_FILENO);

            // Close the slave side of PTY
            close(slave);

            // Execute the specified command
            execl(command.c_str(), command.c_str(), NULL);
        } else {
            // Parent process: Read and write to the PTY
            close(slave);

            // Send input to the PTY
            write(master, input.c_str(), input.size());

            output.clear();

            while (1) {
                int bytesRead = read(master, buffer, sizeof(buffer));
                if (bytesRead <= 0) {
                    break;
                }

                // Append the data read from the PTY to the output
                output.append(buffer, bytesRead);
            }

            // Close the master side of PTY
            close(master);
        }
    }
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // Create the main window
    Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                            BlackPixel(display, screen), WhitePixel(display, screen));
    test_funcs::runShellInXTerm(display, mainWindow, "/home/SERVER/test");

    XMapWindow(display, mainWindow);
    XFlush(display);
}
