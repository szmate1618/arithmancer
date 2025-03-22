#pragma once

#include <vector>
#include <string>

#include "screen.hpp"


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
	// Read a key from the console
	INPUT_RECORD inputRecord;
	DWORD events;
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	ReadConsoleInput(hInput, &inputRecord, 1, &events);

	if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
	{
		int keyCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
		
		if (keyCode == VK_UP)
		{
			std::cout << "Up arrow key pressed!" << std::endl;
		}
	}
}
