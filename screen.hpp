#pragma once

#include <vector>
#include <string>

#include "stringutils.hpp"


class ScreenBuffer {
private:
	std::vector<std::string> buffer;
	size_t width;
	size_t height;

public:
	// Constructor
	ScreenBuffer(size_t w, size_t h) : width(w), height(h), buffer(h, std::string(w, ' ')) {}

	// Getter for width
	size_t getWidth() const {
		return width;
	}

	// Getter for height
	size_t getHeight() const {
		return height;
	}

	// Getter for buffer
	const std::vector<std::string>& getBuffer() const {
		return buffer;
	}

	void setChar(size_t h, size_t w, char c) {
		buffer.at(h).at(w) = c;
	}

	void clear() {
		for (size_t i = 0; i < buffer.size(); i++)
		{
			for (size_t j = 0; j < buffer.at(i).size(); j++)
			{
				buffer.at(i).at(j) = ' ';
			}
		}
	}

	void Display() const
	{
		printf("%s", join(buffer, "\r\n").c_str());
	}
};
