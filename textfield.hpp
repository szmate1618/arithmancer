#pragma once

#include <string>

#include "input.hpp"


class TextField
{
public:
	std::string text;

	void Update(double seconds)
	{
		if (InputHandler::IsPressed(VK_BACK) && !text.empty())
		{
			text.pop_back();
		}
		else
		{
			// Number characters.
			for (char i = 0x30; i <= 0x3A; i++)
			{
				if (InputHandler::IsPressed(i))
				{
					text.push_back(i);
				}
			}
			if (InputHandler::IsPressed('.') && (text.find('.') == std::string::npos))
			{
				text.push_back('.');
			}
		}
	}

	std::string GetText() const
	{
		return text;
	}	
private:
	size_t cursorPosition;
	//size_t maxLength;
};
