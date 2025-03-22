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
