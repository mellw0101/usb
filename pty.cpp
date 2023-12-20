#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pty.h>
#include <string.h>
#include <sys/select.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h> // Include the ioctl library for terminal control

volatile sig_atomic_t reload_flag = 0;

// Signal handler for Ctrl+C
void handle_ctrl_c(int signum) {
    reload_flag = 1;
}

int main() {
    int master, slave;
    char buffer[4096];
    fflush(stdout);

    // Set up signal handling for Ctrl+C
    struct sigaction sa;
    sa.sa_handler = handle_ctrl_c;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Get the current terminal attributes
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);

    // Disable canonical mode and echo for the terminal
    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    while (1) {
        // Open a new PTY
        if (openpty(&master, &slave, NULL, NULL, NULL) == -1) {
            perror("openpty");
            return 1;
        }

        // Fork a child process
        pid_t child = fork();

        if (child == -1) {
            perror("fork");
            return 1;
        } else if (child == 0) {
            // Child process: Replace standard input/output/error with PTY
            close(master);

            // Redirect the slave side of PTY to the child's standard input/output/error
            dup2(slave, STDIN_FILENO);
            dup2(slave, STDOUT_FILENO);
            dup2(slave, STDERR_FILENO);

            // Close the slave side of PTY
            close(slave);

            // Execute a command (e.g., /bin/bash)
            execl("/bin/Shell", "/bin/Shell", NULL);
        } else {
            // Parent process: Read and write to the PTY
            close(slave);

            // Use non-blocking I/O
            int flags = fcntl(master, F_GETFL, 0);
            fcntl(master, F_SETFL, flags | O_NONBLOCK);

            while (1) {
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(STDIN_FILENO, &readfds);
                FD_SET(master, &readfds);

                // Use select to monitor both user input and PTY output
                if (select(master + 1, &readfds, NULL, NULL, NULL) == -1) {
                    perror("select");
                    break;
                }

                if (FD_ISSET(STDIN_FILENO, &readfds)) {
                    // Read user input
                    int bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
                    if (bytesRead <= 0) {
                        break;
                    }

                    // Check for special key sequences
                    if (bytesRead == 3 && buffer[0] == 27 && buffer[1] == 91) {
                        // Handle Up and Down arrow keys (or any other special keys) here
                        // For example, you can process the key sequences and modify behavior accordingly.
                        // To maintain command history, you'll need to implement a history mechanism.
                        // For simplicity, we'll just send the key sequences to the PTY as is.
                    }

                    // Send the user's command to the PTY
                    write(master, buffer, bytesRead);
                }

                if (FD_ISSET(master, &readfds)) {
                    // Read and display the response from the PTY
                    int bytesRead = read(master, buffer, sizeof(buffer));
                    if (bytesRead <= 0) {
                        break;
                    }
                    // Display the response
                    write(STDOUT_FILENO, buffer, bytesRead);
                }
            }

            // Close the master side of PTY
            close(master);

            // Check if the reload flag is set (Ctrl+C pressed)
            if (reload_flag) {
                reload_flag = 0;
                continue; // Restart the main loop
            } else {
                break; // Exit the program
            }
        }
    }

    // Restore the original terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    return 0;
}
