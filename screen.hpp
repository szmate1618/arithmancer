#pragma once

#include <vector>


class ScreenBuffer {
private:
    std::vector<std::vector<char>> buffer;
    int width;
    int height;

public:
    // Constructor
    ScreenBuffer(int w, int h) : width(w), height(h), buffer(h, std::vector<char>(w, ' ')) {}

    // Getter for width
    int getWidth() const {
        return width;
    }

    // Getter for height
    int getHeight() const {
        return height;
    }

    // Getter for buffer
    const std::vector<std::vector<char>>& getBuffer() const {
        return buffer;
    }

    void setChar(size_t h, size_t w, char c) {
        buffer.at(h).at(w) = c;
    }

    void Display() const 
    {
        for (const auto& row : buffer) {
            for (char c : row)
                std::cout << c;
            std::cout << '\n';
        }
    }
};
