#include "menu.hpp"


Menu::Menu()
	: optionTexts({}), optionActions({}), selectedOption(0) {
}

Menu::Menu(const std::vector<std::string>& options, const std::vector<MenuAction>& actions)
	: optionTexts(options), optionActions(actions), selectedOption(0) {
}

void Menu::Draw(ScreenBuffer& screenBuffer) const
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

		if (i == selectedOption) {
			screenBuffer.setChar(startH, startW - 2, '>');
		}
	}
}

void Menu::Update(double seconds)
{
	if (InputHandler::IsUpPressed() || InputHandler::IsPressed('w'))
	{
		selectedOption = (selectedOption + optionTexts.size() - 1) % optionTexts.size();
	}
	if (InputHandler::IsDownPressed() || InputHandler::IsPressed('s'))
	{
		selectedOption = (selectedOption + 1) % optionTexts.size();
	}
	if (InputHandler::IsPressed('\n') || InputHandler::IsPressed('\r'))
	{
		optionActions[selectedOption]();
	}
}
