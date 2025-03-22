#pragma once

#include <format>

#include <windows.h>


void SetConsoleSize(int width, int height) {
    system(std::format("mode {0}, {1}", width, height).c_str());

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set screen buffer size
    COORD coord = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hOut, coord);

    // Set window size
    SMALL_RECT rect = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &rect);
}

void SetCursorPosition(int x, int y) {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position
    COORD coord = { x, y };  // COORD structure stores x and y coordinates
    SetConsoleCursorPosition(hConsole, coord);
}
