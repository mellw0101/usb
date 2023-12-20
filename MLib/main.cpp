#include "String.hpp"
#include "FilO.hpp"
#include <iostream>


int main() {

    mlib::FilO file("/home/mellw/.ShellHistory");

    mlib::String content = "g";

    std::cout << content;

    return 0;
}
