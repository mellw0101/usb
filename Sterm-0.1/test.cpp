#include <iostream>
#include <string>
#include <vector>

void vt102Emulate(const std::string &input) {
    std::vector<std::vector<char>> screen(24, std::vector<char>(80, ' '));  // 24 rows x 80 columns
    int row = 0, col = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '\033') {  // ESC
            ++i;  // Skip the '[' after '\033'
            if (i < input.size() && input[i] == '[') {
                ++i;  // Move to the next character

                std::string code;
                while (i < input.size() && input[i] >= '0' && input[i] <= '9') {
                    code += input[i];
                    ++i;
                }

                if (i < input.size()) {
                    char cmd = input[i];
                    if (cmd == 'A') {
                        // Move cursor up
                        row -= std::stoi(code.empty() ? "1" : code);
                        if (row < 0) row = 0;
                    } else if (cmd == 'B') {
                        // Move cursor down
                        row += std::stoi(code.empty() ? "1" : code);
                        if (row >= 24) row = 23;
                    } else if (cmd == 'C') {
                        // Move cursor forward
                        col += std::stoi(code.empty() ? "1" : code);
                        if (col >= 80) col = 79;
                    } else if (cmd == 'D') {
                        // Move cursor backward
                        col -= std::stoi(code.empty() ? "1" : code);
                        if (col < 0) col = 0;
                    }
                }
            }
        } else if (c == '\n') {
            // Newline
            ++row;
            if (row >= 24) row = 23;
            col = 0;
        } else if (c == '\r') {
            // Carriage return
            col = 0;
        } else {
            // Regular character
            if (row < 24 && col < 80) {
                screen[row][col] = c;
            }
            ++col;
            if (col >= 80) {
                col = 0;
                ++row;
                if (row >= 24) row = 23;
            }
        }
    }

    // Just to display the "screen" for demonstration purposes
    for (const auto &line : screen) {
        for (char c : line) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string input = "Hello, world!\033[5A Moved up 5 lines.";
    vt102Emulate(input);
    return 0;
}
