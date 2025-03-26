#pragma once

#include <vector>
#include <string>


class ScreenBuffer {
private:
	std::vector<std::string> buffer;
	int width;
	int height;

public:
	// Constructor
	ScreenBuffer(int w, int h) : width(w), height(h), buffer(h, std::string(w, ' ')) {}

	// Getter for width
	int getWidth() const {
		return width;
	}

	// Getter for height
	int getHeight() const {
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
		for (size_t i = 0; i < buffer.size(); i++)
		{
			const auto& row = buffer.at(i);
			printf("%s", row.c_str());
			if (i != buffer.size() - 1) printf("\n");
		}
	}
};
