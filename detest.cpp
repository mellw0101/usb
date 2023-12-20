#include <ncurses.h>
#include <vector>
#include <array>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <iostream>
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
namespace desktop_enviroment {
    void de_list(std::vector<std::string> items, std::string& pick) {
        initscr();          // Initialize ncurses
        noecho();           // Don't echo keypresses to the screen
        cbreak();           // Disable line buffering
        keypad(stdscr, TRUE); // Enable function keys, arrow keys, etc.
        curs_set(FALSE);    // Hide the cursor

        int selected = 0;
        int currentState = 0;  // This variable stores the current state

        while (1) {
            clear(); // Clear the screen

            if (currentState == 0) {                            // default state
                // Display the first list
                for (int i = 0; i < items.size(); ++i) {
                    if (i == selected) {
                        attron(A_REVERSE); // Highlight the selected item
                    }
                    mvprintw(i + 1, 1, "%s", items[i].c_str());
                    attroff(A_REVERSE); // Turn off highlight
                }
            } else if (currentState == 1) {                     // when item 1 is chosen do this
                mvprintw(1, 1, "press enter to confirm");
            } else if (currentState == 2) {                     // when item 2 is chosen do this
                mvprintw(1, 1, "press enter to confirm");
            } else if (currentState == 3) {                     // when item 3 is chosen do this
                mvprintw(1, 1, "press enter to confirm");
            } else if (currentState == 10) {
                endwin();
                return;
            }

            int ch = getch();                                   // Get user input
            switch (ch) {
                case KEY_UP:                                    // when KEY_UP is pressed
                    if (selected > 0) {
                        selected--;                             // subtract one from selected
                    }
                    break;
                case KEY_DOWN:                                  // when KEY_DOWN is pressed
                    if (selected < items.size() - 1) {
                        selected++;                             // add 1 to selected
                    }
                    break;
                case '\n':                                      // Enter key
                    if (selected == 0) {                        // if enter is pressed with item 1 highlighted do this
                        if (currentState == 0) {                // enters item 1 if user presses enter from main Menu
                            currentState = 1;
                        } else if (currentState == 1) {         // if user presses enter from item 1 it will return user to main menu
                            pick = "1";
                            currentState = 10;
                        }
                    }
                    if (selected == 1) {                        // if enter is pressed with item 2 highlighted do this
                        if (currentState == 0) {                // enters item 2 if user presses enter from main Menu
                            currentState = 2;
                        } else if (currentState == 2) {         // if user presses enter from item 2 it will return user to main menu
                            pick = "2";
                            currentState = 10;
                        }
                    }
                    if (selected == 2) {                        // if enter is pressed with item 3 highlighted do this
                        if (currentState == 0) {                // enters item 3 if user presses enter from main Menu
                            currentState = 3;
                        } else if (currentState == 3) {         // if user presses enter from item 3 it will return user to main menu
                            pick = "3";
                            currentState = 10;
                        }
                    }
                    break;
                case 'q':
                case 'Q':
                    endwin(); // Terminate ncurses
                    return;
            }

            refresh(); // Update the screen
        }

        endwin(); // Terminate ncurses
    }
    void de_pick() {
        std::string pick;
        std::vector<std::string> items = {"KDE-Plasma", "Gnome", "xfce"};
        de_list(items, pick);

        if (pick == "1") {
            std::cout << "'" << pick << "'\n";
            std::cout << "desktop enviroment KDE-Plasma chosen\n";
        }
        if (pick == "2") {
            std::cout << "'" << pick << "'\n";
            std::cout << "desktop enviroment Gnome chosen\n";
        }
        if (pick == "3") {
            std::cout << "'" << pick << "'\n";
            std::cout << "desktop enviroment xfce chosen\n";
        }
    }
}




int main() {

    desktop_enviroment::de_pick();
    return 0;
}
