#include <iostream>
#include <string>

// Function to prompt the user and get input
std::string promptUser(const std::string& prompt) {
    std::string userInput;
    std::cout << prompt;
    std::getline(std::cin, userInput); // Read a line of text from the user
    return userInput;
}

int main() {
    // Prompt the user for their name
    std::string name = promptUser("Please enter your name: ");

    // Display a greeting
    std::cout << "Hello, " << name << "!" << std::endl;

    return 0;
}
