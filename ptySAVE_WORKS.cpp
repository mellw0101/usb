#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pty.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <signal.h>

int main() {
    int master, slave;
    char buffer[4096];
    fflush(stdout);

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
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    break;
                }

                // Check for exit condition
                if (strcmp(buffer, "exit\n") == 0) {
                    break;
                }

                // Send the user's command to the PTY
                write(master, buffer, strlen(buffer));
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
    }

    return 0;
}
