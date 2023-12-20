#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <spawn.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
#include <regex>

namespace executefunctions {
    namespace example1 {
        #include <iostream>
        #include <vector>
        #include <cstring>
        #include <stdexcept>
        #include <spawn.h>
        #include <sys/wait.h>

        void runExecutable(const std::string& program, const std::vector<std::string>& args) {
            posix_spawn_file_actions_t file_actions;
            posix_spawn_file_actions_init(&file_actions);

            // Create an array of C-style strings (char*) for execvp
            std::vector<const char*> argv;
            argv.push_back(program.c_str()); // First argument is the program name
            for (const std::string& arg : args) {
                argv.push_back(arg.c_str());
            }
            argv.push_back(nullptr); // Null-terminate the array

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
                perror("posix_spawn");
                throw std::runtime_error("Failed to spawn process");
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                throw std::runtime_error("Error waiting for process");
            }

            if (WIFEXITED(status)) {
                int exitStatus = WEXITSTATUS(status);
                std::cout << "Process exited with status: " << exitStatus << std::endl;
            } else {
                std::cerr << "Process did not exit normally." << std::endl;
            }
        } int howtorun() {
            std::vector<std::string> args = {"ls", "-l"};
            try {
                runExecutable("/bin/ls", args);
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }

            return 0;
        }
    }
    namespace char_exaple {
        char runExecutable(const std::string& program, const std::vector<std::string>& args) {
            posix_spawn_file_actions_t file_actions;
            posix_spawn_file_actions_init(&file_actions);

            struct FileActionsRAII {
                posix_spawn_file_actions_t* fa;
                FileActionsRAII(posix_spawn_file_actions_t* fa) : fa(fa) {}
                ~FileActionsRAII() { posix_spawn_file_actions_destroy(fa); }
            } fileActionsRAII(&file_actions);

            std::vector<const char*> argv(args.size() + 2, nullptr);
            argv[0] = program.c_str();
            for (size_t i = 0; i < args.size(); ++i) {
                argv[i + 1] = args[i].c_str();
            }

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
                return -1; // Error spawning process
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                return -2; // Error waiting for process
            }

            if (WIFEXITED(status)) {
                return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
            } else {
                return -3; // Process did not exit normally
            }
        }

        int main() {
            std::string program = "/bin/ls"; // Example program (e.g., ls)
            std::vector<std::string> args = {"-l", "-a", "/path/to/directory"};

            char result = runExecutable(program, args);

            if (result == -1) {
                std::cerr << "Error spawning process." << std::endl;
            } else if (result == -2) {
                std::cerr << "Error waiting for process." << std::endl;
            } else if (result == -3) {
                std::cerr << "Process did not exit normally." << std::endl;
            } else {
                std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
            }

            return result;
        }
    }
    namespace char_single {
        #include <iostream>
        #include <cstring>
        #include <unistd.h>
        #include <sys/wait.h>
        #include <spawn.h>

        extern char** environ; // Declare the external environ variable

        char runExecutable(const std::string& program) {
            char* empty_env[] = {nullptr};
            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, empty_env, environ) != 0) {
                return -1; // Error spawning process
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                return -2; // Error waiting for process
            }

            if (WIFEXITED(status)) {
                return static_cast<char>(WEXITSTATUS(status)); // Return the exit status as a char
            } else {
                return -3; // Process did not exit normally
            }
        }

        int main() {
            std::string program = "/bin/ls"; // Example program (e.g., ls)

            char result = runExecutable(program);

            if (result == -1) {
                std::cerr << "Error spawning process." << std::endl;
            } else if (result == -2) {
                std::cerr << "Error waiting for process." << std::endl;
            } else if (result == -3) {
                std::cerr << "Process did not exit normally." << std::endl;
            } else {
                std::cout << "Process exited with status: " << static_cast<int>(result) << std::endl;
            }

            return result;
        }

    }
    namespace example_7 {
        #include <iostream>
        #include <vector>
        #include <cstring>
        #include <unistd.h>
        #include <sys/wait.h>
        #include <spawn.h>
        #include <cstdlib>

        int runExecutable(const std::string& program, const std::vector<std::string>& args) {
            posix_spawn_file_actions_t file_actions;
            posix_spawn_file_actions_init(&file_actions);

            std::vector<const char*> argv;
            argv.reserve(args.size() + 2); // Reserve space to avoid reallocations
            argv.push_back(program.c_str());
            for (const auto& arg : args) {
                argv.push_back(arg.c_str());
            }
            argv.push_back(nullptr);

            pid_t pid;
            if (posix_spawn(&pid, program.c_str(), &file_actions, NULL, const_cast<char* const*>(argv.data()), environ) != 0) {
                perror("posix_spawn");
                return -1;
            }

            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                return -2;
            }

            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return -3;
            }
        }

        int main() {
            std::string program = "/bin/ls"; // Example program (e.g., ls)
            std::vector<std::string> args = {"-l", "-a", "/path/to/directory"};

            int result = runExecutable(program, args);

            if (result == -1) {
                std::cerr << "Error spawning process." << std::endl;
            } else if (result == -2) {
                std::cerr << "Error waiting for process." << std::endl;
            } else if (result == -3) {
                std::cerr << "Process did not exit normally." << std::endl;
            } else {
                std::cout << "Process exited with status: " << result << std::endl;
            }

            return 0;
        }

    }
    namespace open_pipe {
        #include <iostream>
        #include <cstdio>

        int main() {
            FILE* pipe = popen("ls -l", "r");
            if (pipe) {
                char buffer[128];
                while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                    std::cout << buffer;
                }
                pclose(pipe);
            }
            return 0;
        }

    }
    namespace pipe_linux_to_windows {
        namespace sender_cpp {
            #include <iostream>
            #include <fstream>
            #include <string>
            #include <unistd.h>

            int main() {
                const char* executablePath = "./receiver.exe"; // Replace with the path to your receiver.exe
                const char* dataToSend = "Hello, receiver!";   // Data to send

                int pipefd[2]; // Pipe file descriptors

                // Create a pipe
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    return 1;
                }

                // Fork a child process
                pid_t childPid = fork();

                if (childPid == -1) {
                    perror("fork");
                    return 1;
                }

                if (childPid == 0) {
                    // Child process
                    close(pipefd[1]); // Close the write end of the pipe
                    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
                    close(pipefd[0]); // Close the read end of the pipe

                    // Execute the receiver.exe
                    execl(executablePath, executablePath, NULL);

                    // If execl fails
                    perror("execl");
                    return 1;
                } else {
                    // Parent process
                    close(pipefd[0]); // Close the read end of the pipe

                    // Send data to the child process
                    write(pipefd[1], dataToSend, strlen(dataToSend));
                    close(pipefd[1]); // Close the write end of the pipe

                    // Wait for the child process to finish (optional)
                    int status;
                    waitpid(childPid, &status, 0);

                    std::cout << "Data sent to receiver.exe." << std::endl;
                }

                return 0;
            }

        }
        namespace receiver_c {
            #include <stdio.h>
            #include <string.h>

            int main() {
                char buffer[256];
                ssize_t bytesRead;

                // Read data from stdin (which is redirected from the pipe)
                while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
                    buffer[bytesRead] = '\0'; // Null-terminate the string
                    printf("Received data: %s\n", buffer);
                }

                return 0;
            }
        }
    }
    namespace stdcout_to_file {
        int outFile = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        std::string dup2(outFile, STDOUT_FILENO);
    }
    namespace grep_find {
        bool search_string(std::string& targetstring, const std::string& wordToFind) {
            // Create a regular expression pattern to match the whole word
            std::regex wordPattern("\\b[^\\s]*" + wordToFind + "[^\\s]*\\b");
            //std::regex wordPattern("\\b" + wordToFind + "\\b");
            // Search for and highlight words containing 'b'
            std::string highlightedInput = std::regex_replace(targetstring, wordPattern, "\033[1;31m$&\033[0m");
            if (highlightedInput == "") {
                return false;
            } else {
                // Print the highlighted input
                std::cout << highlightedInput << std::endl;
                return true;
            }
        }
        bool search_string_test(std::string& targetstring, const std::string& wordToFind) {
            // Create a regular expression pattern to match the whole word
            std::regex wordPattern("\\b" + wordToFind + "\\b");

            // Search for the word in the target string
            std::smatch match;
            if (std::regex_search(targetstring, match, wordPattern)) {
                std::string highlightedInput;

                // Construct the highlighted output
                highlightedInput += targetstring.substr(0, match.position()); // Part before the match
                highlightedInput += "\033[1;31m" + match.str() + "\033[0m"; // Highlighted match
                highlightedInput += targetstring.substr(match.position() + match.length()); // Part after the match

                // Print the highlighted input
                std::cout << highlightedInput << std::endl;
                return true;
            } else {
                return false;
            }
        }
    }
    namespace html_paco {
        #include <iostream>
        #include <string>
        #include <Poco/Net/HTTPClientSession.h>
        #include <Poco/Net/HTTPRequest.h>
        #include <Poco/Net/HTTPResponse.h>
        #include <Poco/StreamCopier.h>
        #include <Poco/URI.h>

        // Function to fetch HTML content from a URL using Poco
        std::string fetchHtmlFromUrl(const std::string& url) {
            Poco::URI uri(url);
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
            std::string path(uri.getPathAndQuery());

            if (path.empty()) {
                path = "/";
            }

            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            Poco::Net::HTTPResponse response;

            try {
                session.sendRequest(request);
                std::istream& rs = session.receiveResponse(response);

                std::string htmlContent;
                Poco::StreamCopier::copyToString(rs, htmlContent);

                return htmlContent;
            } catch (Poco::Exception& ex) {
                std::cerr << "Poco error: " << ex.displayText() << std::endl;
                return "";
            }
        }

        int main() {
            // URL of the website to fetch HTML content from
            std::string url = "https://example.com";

            // Fetch HTML content from the URL using Poco
            std::string htmlContent = fetchHtmlFromUrl(url);

            // Print the HTML content
            std::cout << "HTML Content from " << url << ":\n" << htmlContent << std::endl;

            return 0;
        }
    }
    namespace Sterminal {
        #include <X11/Xlib.h>
        #include <X11/Xutil.h>
        #include <curses.h>
        #include <readline/chardefs.h>
        #include <spawn.h>
        #include <string>
        #include <iostream>
        #include <sys/wait.h>
        #include <unistd.h>
        #include <vector>
        #include <fstream>
        #include <iostream>  // Include the header for std::cout
        #include <sstream>   // Include the header for std::ostringstream
        #include <cstdlib>
        #include <ncurses.h>
        #include <X11/extensions/Xrandr.h>
        #include <X11/Xutil.h>
        #include <cstring>
        #include <X11/keysym.h>
        #include <iostream>
        #include <pty.h>
        #include <unistd.h>
        #include <string>



        namespace programPaths {
            std::string xterm = "/usr/bin/xterm";
            std::string konsole = "/usr/bin/konsole";
        }

        namespace windowFunctions {
            void createBorderlessWindow(Display* display, Window& window, int width, int height) {
                int screen = DefaultScreen(display);
                Window root = RootWindow(display, screen);

                // Create the main window with no borders
                window = XCreateSimpleWindow(display, root, 0, 0, width, height, 0, 0, WhitePixel(display, screen));

                // Set the window manager hints to remove decorations
                XSizeHints sizeHints;
                sizeHints.flags = PPosition | PSize;
                sizeHints.x = 0;
                sizeHints.y = 0;
                sizeHints.width = width;
                sizeHints.height = height;

                XWMHints wmHints;
                wmHints.flags = InputHint;
                wmHints.input = True;

                XClassHint classHint;
                classHint.res_name = const_cast<char*>("borderless_window");
                classHint.res_class = const_cast<char*>("BorderlessWindow");

                XSetWMProperties(display, window, nullptr, nullptr, nullptr, 0, &sizeHints, &wmHints, &classHint);

                // Map and flush the window
                XMapWindow(display, window);
                XFlush(display);
            }
            void runProgramWargs(const std::string &program, const std::vector<std::string> &args) {
                std::vector<char *> argv;
                for (const std::string &arg : args) {
                    argv.push_back(const_cast<char *>(arg.c_str()));
                }
                argv.push_back(nullptr);

                if (fork() == 0) {
                    // Child process
                    execvp(program.c_str(), argv.data());
                    perror("execvp failed"); // Print an error message if execvp fails
                    exit(1);
                } else {
                    // Parent process
                    wait(NULL); // Wait for the child process to finish
                }
            }
            void runProgram(const std::string& program) {
                    if (fork() == 0) {
                        // Child process
                        std::string test = "xterm";
                        execlp(test.c_str(), program.c_str(), nullptr);
                        perror("execlp failed"); // Print an error message if execlp fails
                        exit(1);
                    } else {
                        // Parent process
                        wait(NULL); // Wait for the child process to finish
                    }
            }
            void createVirtualDisplay(int width, int height, const std::string& displayNumber) {
                std::ostringstream command;
                command << "Xephyr -screen " << width << "x" << height << " :" << displayNumber << " &";

                system(command.str().c_str());
                return;
            }
            void runOnVirtualDisplay(const std::string& displayNumber, const std::string& programPath) {
                std::ostringstream command;
                command << "DISPLAY=:" << displayNumber << " " << programPath;
                system(command.str().c_str());
                return;
            }
            void runOnVirtualDisplaytest(const std::string& displayNumber, const std::string& programPath) {

                std::string basecommand = "DISPLAY=:" + displayNumber;
                runProgramWargs(basecommand, {basecommand, programPaths::xterm, "-e", programPath});
                return;
            }
            bool startVirtualXServer_Xvfb(const std::string& displayNumber) {
                std::string command = "Xvfb :"+displayNumber+" -screen 0 1024x768x24 &";
                int result = system(command.c_str());

                if (result != 0) {
                    std::cerr << "Error: Unable to start virtual X server." << std::endl;
                    return false;
                }

                return true;
            }
            bool startVirtualXServer_Xephyr(const std::string& displayNumber) {
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
            void runShellInXTerm(Display* display, Window window, const std::string& programPath) {
                    // Run your shell in the xterm within the shell window
                    std::string command = "xterm -into " + std::to_string(static_cast<unsigned long>(window)) + " -e " + programPath;
                    runProgram(command.c_str());
            }
            void runShellInXTermtest(Display* display, Window window, const std::string& programPath) {
                    // Run your shell in the xterm within the shell window
                    std::string name = "shell";
                    //std::to_string(window);
                    //std::string windowset = std::to_string(static_cast<unsigned long>(window));
                    std::string program = programPath;
                    runProgramWargs(programPaths::xterm, {"xterm","-T", name, "-fn", "fixed", program});
                    XMapWindow(display, window);

            }
            void runXtermOnVirtualDisplay(const std::string& displayNumber) {
                // Start the Xvfb server on the specified display number
                std::string xvfbCommand = "Xvfb :" + displayNumber + " -screen 0 800x600x16 &";
                int xvfbResult = system(xvfbCommand.c_str());

                if (xvfbResult != 0) {
                    std::cerr << "Error: Failed to start Xvfb." << std::endl;
                    return;
                }

                // Set the DISPLAY environment variable to use the virtual display
                setenv("DISPLAY", (":" + displayNumber).c_str(), 1);

                // Run xterm on the virtual display
                std::string xtermCommand = "xterm";
                int xtermResult = system(xtermCommand.c_str());

                if (xtermResult != 0) {
                    std::cerr << "Error: Failed to run xterm." << std::endl;
                    return;
                }

                // Wait for the user to press Enter before exiting
                std::cout << "Press Enter to exit...";
                std::cin.get();

                // Clean up: Stop the Xvfb server
                std::string stopXvfbCommand = "pkill Xvfb";
                system(stopXvfbCommand.c_str());
            }
            void runProgramInWindow(Display* display, Window window, const std::string& programPath) {
                // Fork a child process
                pid_t childPid = fork();

                if (childPid == -1) {
                    std::cerr << "Error: Fork failed." << std::endl;
                    return;
                }

                if (childPid == 0) { // Child process
                    // Close the X11 display in the child process
                    XCloseDisplay(display);

                    // Execute the program in the child process
                    execlp(programPath.c_str(), programPath.c_str(), nullptr);

                    // If execlp fails, print an error message and exit
                    std::cerr << "Error: execlp failed." << std::endl;
                    exit(1);
                } else { // Parent process
                    // Wait for the child process to exit
                    int status;
                    waitpid(childPid, &status, 0);
                }
            }
            void runProgramInWindow(Display* display, Window window, const std::string& programPath, const std::vector<std::string>& arguments) {
                // Fork a child process
                pid_t childPid = fork();

                if (childPid == -1) {
                    std::cerr << "Error: Fork failed." << std::endl;
                    return;
                }

                if (childPid == 0) { // Child process
                    // Close the X11 display in the child process
                    XCloseDisplay(display);

                    // Create an array of C-style strings for execlp
                    std::vector<const char*> args;
                    args.push_back(programPath.c_str()); // The first argument is the program itself

                    for (const std::string& arg : arguments) {
                        args.push_back(arg.c_str());
                    }

                    args.push_back(nullptr); // Null-terminate the array

                    // Execute the program with arguments in the child process
                    execvp(programPath.c_str(), const_cast<char* const*>(&args[0]));

                    // If execvp fails, print an error message and exit
                    std::cerr << "Error: execvp failed." << std::endl;
                    exit(1);
                } else { // Parent process
                    // Wait for the child process to exit
                    int status;
                    waitpid(childPid, &status, 0);
                }
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
            void displayTextInEmptyWindow(const std::string& text) {
                Display* display = XOpenDisplay(nullptr);
                if (!display) {
                    std::cerr << "Error: X11 display initialization failed." << std::endl;
                    return;
                }

                int screen = DefaultScreen(display);
                Window rootWindow = RootWindow(display, screen);

                // Create an empty window
                Window window = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                                    BlackPixel(display, screen), WhitePixel(display, screen));
                XMapWindow(display, window);
                XFlush(display);

                // Create a graphics context
                XGCValues values;
                GC gc = XCreateGC(display, window, 0, &values);

                // Set the font and font color (white)
                XFontStruct* fontInfo = XLoadQueryFont(display, "fixed");
                XSetFont(display, gc, fontInfo->fid);
                XSetForeground(display, gc, WhitePixel(display, screen));

                // Set the text position
                int x = 100;
                int y = 100;

                // Render the text
                XDrawString(display, window, gc, x, y, text.c_str(), text.length());
                XMapWindow(display, window);
                XFlush(display);

                // Event loop or other code here

                // Close the X11 display (not reached in this example)
                // XCloseDisplay(display);
            }
            namespace ncursesTerminal {
                WINDOW* initializeNcurses() {
                    initscr();          // Initialize ncurses
                    raw();              // Disable line buffering
                    keypad(stdscr, TRUE); // Enable special keys
                    noecho();           // Don't print typed characters
                    curs_set(0);        // Hide the cursor
                    return 0;
                }
                void cleanupNcurses(WINDOW* win) {
                    // Additional cleanup if needed

                    delwin(win); // Delete the NCurses window

                    endwin();    // Clean up NCurses
                }
                void runTerminal(WINDOW* win) { // Pass the WINDOW* as a parameter
                    std::vector<std::string> history;
                    int historyIndex = -1;
                    std::string currentInput;

                    wprintw(win, "Welcome to My Terminal Emulator!\n");
                    wprintw(win, "Press F12 to exit.\n");
                    wrefresh(win);

                    while (true) {
                        int ch = wgetch(win);

                        if (ch == KEY_F(12)) {
                            break; // Exit on F12 key
                        } else if (ch == '\n') {
                            // Execute command (you can replace this with your own command execution logic)
                            wprintw(win, "Command: %s\n", currentInput.c_str());
                            wrefresh(win);
                            history.push_back(currentInput);
                            historyIndex = -1;
                            currentInput.clear();
                        } else if (ch == KEY_BACKSPACE || ch == 127) {
                            // Handle backspace
                            if (!currentInput.empty()) {
                                currentInput.pop_back();
                                wprintw(win, "\b \b"); // Move cursor back and clear character
                                wrefresh(win);
                            }
                        } else if (ch == KEY_UP) {
                            // Navigate command history (up)
                            if (!history.empty() && historyIndex < static_cast<int>(history.size()) - 1) {
                                if (historyIndex == -1) {
                                    historyIndex = static_cast<int>(history.size()) - 1;
                                    wprintw(win, "%s", currentInput.c_str());
                                    wrefresh(win);
                                } else {
                                    historyIndex++;
                                    wprintw(win, "\r%*s\r%s", COLS - 1, " ", history[historyIndex].c_str());
                                    currentInput = history[historyIndex];
                                    wrefresh(win);
                                }
                            }
                        } else if (ch == KEY_DOWN) {
                            // Navigate command history (down)
                            if (!history.empty() && historyIndex >= 0) {
                                historyIndex--;
                                if (historyIndex == -1) {
                                    wprintw(win, "\r%*s\r%s", COLS - 1, " ", currentInput.c_str());
                                    currentInput.clear();
                                    wrefresh(win);
                                } else {
                                    wprintw(win, "\r%*s\r%s", COLS - 1, " ", history[historyIndex].c_str());
                                    currentInput = history[historyIndex];
                                    wrefresh(win);
                                }
                            }
                        } else {
                            // Handle regular character input
                            currentInput += static_cast<char>(ch);
                            waddch(win, ch);
                            wrefresh(win);
                        }
                    }
                }
            }
            using namespace ncursesTerminal;
            void removeWindowDecorations(Display* display, Window window) {
                Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", False);
                if (mwmHintsProperty != None) {
                    unsigned long hints[5] = {2, 0, 0, 0, 0}; // See MotifWMHints documentation
                    XChangeProperty(display, window, mwmHintsProperty, mwmHintsProperty, 32, PropModeReplace, (unsigned char*)hints, 5);
                    XFlush(display);
                }
            }
            void runProgramInWindow(Display* display, Window window, const std::string& programPath, const std::vector<std::string>& arguments, const std::string& windowId) {
                // Fork a child process
                pid_t childPid = fork();

                if (childPid == -1) {
                    std::cerr << "Error: Fork failed." << std::endl;
                    return;
                }

                if (childPid == 0) { // Child process
                    // Close the X11 display in the child process
                    XCloseDisplay(display);

                    // Create an array of C-style strings for execlp
                    std::vector<const char*> args;
                    args.push_back(programPath.c_str()); // The first argument is the program itself

                    // Add window ID argument
                    args.push_back("-into");
                    args.push_back(windowId.c_str());

                    // Add other provided arguments
                    for (const std::string& arg : arguments) {
                        args.push_back(arg.c_str());
                    }

                    args.push_back(nullptr); // Null-terminate the array

                    // Execute the program with arguments in the child process
                    execvp(programPath.c_str(), const_cast<char* const*>(&args[0]));

                    // If execvp fails, print an error message and exit
                    std::cerr << "Error: execvp failed." << std::endl;
                    exit(1);
                } else { // Parent process
                    // Wait for the child process to exit
                    int status;
                    waitpid(childPid, &status, 0);
                }
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
        void SendInputToPTY(int master, const std::string& input) {
            // Write the input data to the master side of PTY
            write(master, input.c_str(), input.size());
        }

        std::string PTy(std::string input){
            int master, slave;
            char buffer[4096];

            // Open a new PTY
            if (openpty(&master, &slave, NULL, NULL, NULL) == -1) {
                perror("openpty");
                return "";
            }

            // Fork a child process
            pid_t child = fork();

            if (child == -1) {
                perror("fork");
                return "";
            } else if (child == 0) {
                // Child process: Replace standard input/output/error with PTY
                close(master);

                // Redirect the slave side of PTY to the child's standard input/output/error
                dup2(slave, STDIN_FILENO);
                dup2(slave, STDOUT_FILENO);
                dup2(slave, STDERR_FILENO);

                // Close the slave side of PTY
                close(slave);

                //execl("/bin/bash", "/bin/bash", NULL);
                // Execute a command (e.g., /bin/bash)
                execlp("/home/SERVER/test", "/home/SERVER/test", NULL);
            } else {
                // Parent process: Read and write to the PTY
                close(slave);

                // Send input to the PTY (e.g., "ls\n")
                SendInputToPTY(master, input);

                while (1) {
                    int bytesRead = read(master, buffer, sizeof(buffer));
                    if (bytesRead <= 0) {
                        return "";
                        break;
                    }

                    // Handle the data read from the PTY as needed (e.g., print it)
                    write(STDOUT_FILENO, buffer, bytesRead);
                    if (getcwd(buffer, sizeof(buffer)) != NULL) {
                    return std::string(buffer); // Convert the C-string to a C++ string
                } else {
                    std::cerr << "Error getting current working directory: " << strerror(errno) << std::endl;
                    return ""; // Return an empty string on error
                }
                }

                // Close the master side of PTY
                close(master);
            }
            return "";
        }


        void displayText(Display* display, Window window, const char* text) {
            XClearWindow(display, window);
            XDrawString(display, window, DefaultGC(display, DefaultScreen(display)), 10, 30, text, strlen(text));
            XFlush(display);
        }
        void displayOutput(Display* display, Window window, const std::string& output) {
            displayText(display, window, output.c_str());
        }

        // Function to split a single string into lines
        std::vector<std::string> SplitStringIntoLines(const std::string& input, int lineLength) {
            std::vector<std::string> lines;
            std::istringstream iss(input);
            std::string line;

            while (std::getline(iss, line)) {
                if (line.length() > static_cast<size_t>(lineLength)) {
                    // If a line is longer than the specified lineLength, split it into multiple lines
                    size_t startPos = 0;
                    while (startPos < line.length()) {
                        lines.push_back(line.substr(startPos, lineLength));
                        startPos += lineLength;
                    }
                } else {
                    lines.push_back(line);
                }
            }

            return lines;
        }
        // Overloaded version to split a vector of strings into lines
        std::vector<std::string> SplitStringIntoLines(const std::vector<std::string>& input, int lineLength) {
            std::vector<std::string> lines;
            for (const std::string& str : input) {
                std::vector<std::string> subLines = SplitStringIntoLines(str, lineLength);
                lines.insert(lines.end(), subLines.begin(), subLines.end());
            }
            return lines;
        }

        // Overloaded version to split a char* (C-string) into lines
        std::vector<std::string> SplitStringIntoLines(const char* input, int lineLength) {
            std::string str(input);
            return SplitStringIntoLines(str, lineLength);
        }


        int main(int argc, char* argv[]) {
            const std::string argu_1 = argv[0];
            if (argc == 2) {
                const std::string argu_2 = argv[1];
                if (argu_2 == "konsole") {
                        windowFunctions::createVirtualDisplay(int(1022), int(665), "88");
                        system("sleep 0.2");
                        windowFunctions::runOnVirtualDisplay("88", programPaths::konsole);
                } else if (argu_2 == "mkwin") {
                    const std::string argu_3 = argv[2];
                    windowFunctions::createAndRenderTextWindow(argu_3);
                } else if (argu_2 == "text") {
                    windowFunctions::displayTextInEmptyWindow("balle");
                } else if (argu_2 == "ncurses") {
                Display* display = XOpenDisplay(NULL);
                    if (!display) {
                        fprintf(stderr, "Error: Unable to open X display.\n");
                        return 1;
                    }

                    int screen = DefaultScreen(display);
                    Window root = RootWindow(display, screen);

                    Window window = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 0, BlackPixel(display, screen), WhitePixel(display, screen));

                    XSelectInput(display, window, StructureNotifyMask);
                    XMapWindow(display, window);
                    XFlush(display);
                    XEvent event;
                    system("sleep 2");
                    WINDOW* ncursesWin = windowFunctions::initializeNcurses();
                    windowFunctions::runTerminal(ncursesWin);           // Pass the X11 window to the terminal function
                    windowFunctions::cleanupNcurses(ncursesWin);
                }
            } else if (argc >= 3) {
                    const std::string argu_2 = argv[1];
                    const std::string argu_3 = argv[2];
                    if (argu_2 == "konsole") {
                        windowFunctions::createVirtualDisplay(int(1022), int(665), argu_3);
                        system("sleep 0.2");
                        windowFunctions::runOnVirtualDisplay(argu_3, programPaths::konsole);
                    } else if (argu_2 == "test2") {
                        windowFunctions::runXtermOnVirtualDisplay(argu_3);
                    } else {
                        return 0;
                    }
            } else if (argc == 1){
                Display* display = XOpenDisplay(nullptr);
                if (!display) {
                    std::cerr << "Error: X11 display initialization failed." << std::endl;
                    return 1;
                }
                int lineLength = 20;
                int screen = DefaultScreen(display);
                Window rootWindow = RootWindow(display, screen);
                Window mainWindow = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 1400, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));
                //Window mainWindow = XCreateSimpleWindow(display, rootWindow, 0, 0, 800, 600, 1,
                                                    //BlackPixel(display, screen), WhitePixel(display, screen));
                // Register for events on the window
                XSelectInput(display, mainWindow, ExposureMask | KeyPressMask);

                // Map the window to the screen
                XMapWindow(display, mainWindow);

                XFontStruct *font_info = XLoadQueryFont(display, "fixed");
                GC gc = XCreateGC(display, mainWindow, 0, NULL);
                XSetFont(display, gc, font_info->fid);

                XEvent event;
                char userInput[256] = "";
                int userInputLength = 0;
                std::vector<std::string> lines;                // Vector to store lines from the output
                std::string output = "";


                while (1) {
                    XNextEvent(display, &event);

                    if (event.type == Expose) {
                        int yOffset = 20;                           //initial y potition

                        // Draw each line from the vector
                        for (const std::string& line : lines) {
                            XDrawString(display, mainWindow, gc, 20, yOffset, line.c_str(), line.length());
                            yOffset += 20; // Increment the Y position for the next line
                        }
                        XFlush(display);

                    } else if (event.type == KeyPress) {
                        char keybuf[32];
                        KeySym key;
                        XComposeStatus compose;
                        XLookupString(&event.xkey, keybuf, sizeof(keybuf), &key, &compose);


                        if (key == XK_Return) {
                            // User pressed Enter, clear the input buffer
                            std::string test = userInput;
                            std::string ptyin = test + "\n";
                            std::string output = PTy(ptyin);


                            // Split the output into lines and store them in the 'lines' vector
                            std::vector<std::string> singleStringLines = SplitStringIntoLines(output, lineLength);

                            // Add the lines from 'singleStringLines' to 'lines'
                            lines.insert(lines.end(), singleStringLines.begin(), singleStringLines.end());
                            userInput[0] = '\0';
                            userInputLength = 0;
                        } else if (userInputLength < sizeof(userInput) - 1) {
                            // Append the pressed key to the input buffer
                            userInput[userInputLength] = keybuf[0];
                            userInput[userInputLength + 1] = '\0';
                            userInputLength++;
                        }
                        displayText(display, mainWindow, userInput);
                        XFlush(display);
                    }
                }

            // Close the X display when you're done
            XCloseDisplay(display);

            return 0;
            }
        }

    }
}

