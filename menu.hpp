#pragma once

#include <vector>
#include <string>

#include "screen.hpp"
#include "input.hpp"


class Menu
{
public:
	Menu();
	void Draw(ScreenBuffer& screenBuffer);
	void Update();
private:
	enum class Option
	{
		NEW_GAME,
		QUIT
	};

	const std::vector<std::string> optionTexts = {"New game", "Quit"};

	Option selectedOption;
};

Menu::Menu(): selectedOption(Option::NEW_GAME) {}

void Menu::Draw(ScreenBuffer& screenBuffer)
{
	for (size_t i = 0; i < screenBuffer.getHeight(); i++)
	{
		for (size_t j = 0; j < screenBuffer.getWidth(); j++)
		{
			screenBuffer.setChar(i, j, ' ');
		}
	}

	size_t optionCount = optionTexts.size();
	for (size_t i = 0; i < optionTexts.size(); i++)
	{
		size_t optionLength = optionTexts[i].length();
		size_t startW = screenBuffer.getWidth() / 2 - optionLength / 2;
		size_t startH = screenBuffer.getHeight() / 2 - optionCount + i * 2;

		for (size_t j = 0; j < optionLength; j++)
		{
			screenBuffer.setChar(startH, startW + j, optionTexts[i][j]);
		}

		if (static_cast<Option>(i) == selectedOption) {

			screenBuffer.setChar(startH, startW - 2, '>');
		}
	}
}

void Menu::Update()
{
	if (InputHandler::IsUpPressed() || InputHandler::IsPressed('w'))
	{
		selectedOption = static_cast<Option>((static_cast<int>(selectedOption) + 1) % optionTexts.size());
	}
	if (InputHandler::IsDownPressed() || InputHandler::IsPressed('s'))
	{
		selectedOption = static_cast<Option>((static_cast<int>(selectedOption) - 1) % optionTexts.size());
	}
}
