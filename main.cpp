#include <iostream>

#include "windowing.hpp"


int main() {
    SetConsoleSize(100, 100);
    std::cout << "Console resized!" << std::endl;
    return 0;
}