#include <iostream>

#define NOMINMAX
#include "windowing.hpp"
#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"


int main() {
    SetConsoleSize(100, 50);

    Map map(100, 50, 20);
    map.PrintDungeon();

    ScreenBuffer screenBuffer(100, 50);
    Menu menu;
    menu.Draw(screenBuffer);
    screenBuffer.Display();

    return 0;
}