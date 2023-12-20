#include <libtorrent/session.hpp>
#include <ncurses.h>
#include <iostream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>

void download_torrent(lt::session& sess, const std::string& magnet_uri) {
    // Prepare the parameters for the new torrent
    lt::add_torrent_params atp;
    lt::error_code ec;
    parse_magnet_uri(magnet_uri, atp, ec);

    // Add torrent to the session
    lt::torrent_handle th = sess.add_torrent(atp);

    while (true) {
        // Fetch alerts
        std::vector<lt::alert*> alerts;
        sess.pop_alerts(&alerts);

        // Handle alerts
        for (const auto& alert : alerts) {
            std::cout << alert->message() << std::endl;
        }

        // Check torrent status
        lt::torrent_status status = th.status();
        if (status.is_finished) {
            std::cout << "Download completed" << std::endl;
            return;
        }

        // Wait for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void list(std::vector<std::string> items, std::string& pick) {
    char magnetLink[256];  // Buffer for the magnet link
    lt::session sess;
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
            mvprintw(1, 1, "press enter to confirm KDE as desktop enviroment");
        } else if (currentState == 2) {                     // when item 2 is chosen do this
            mvprintw(1, 1, "press enter to confirm Gnome as desktop enviroment");
        } else if (currentState == 3) {                     // when item 3 is chosen do this
            mvprintw(1, 1, "press enter to confirm xfce as desktop enviroment");
        } else if (currentState == 4) {
            echo(); // Enable echoing
            mvprintw(1, 1, "Input Magnet Link: ");
            // char magnetLink[256];  // Buffer for the magnet link
            mvscanw(1, 20, "%s", magnetLink);
            noecho(); // Disable echoing
            currentState = 1;  // Go back to the main menu
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
                        currentState = 4;
                    } else if (currentState == 1) {         // if user presses enter from item 1 it will return user to main menu
                        std::string link(magnetLink);
                        std::string magnet_uri = "magnet:?xt=urn:btih:" + link;
                        download_torrent(sess, magnet_uri);

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
        }
        refresh(); // Update the screen
    }
    endwin(); // Terminate ncurses
}

int main() {
    std::vector<std::string> items = {"add", "downloading", "finished"};
    std::string pick;
    list(items, pick);
}
