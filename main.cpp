#include <iostream>

#define NOMINMAX
#include "windowing.hpp"
#include "map.hpp"


int main() {
    SetConsoleSize(100, 100);

    Map map(20);
    map.PrintDungeon();

    return 0;
}