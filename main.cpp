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
    while (true) {
        InputHandler::Update();
        menu.Update();
        menu.Draw(screenBuffer);
        SetCursorPosition(0, 0);
        screenBuffer.Display();
    }

    return 0;
}