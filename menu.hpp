#pragma once

#include <vector>
#include <string>
#include <functional>

#include "screen.hpp"
#include "input.hpp"

class Menu
{
public:
	using MenuAction = std::function<void()>;

	Menu();
	Menu(const std::vector<std::string>& options, const std::vector<std::function<void()>>& actions);
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);

private:
	enum class Option
	{
		NEW_GAME,
		QUIT
	};

	std::vector<std::string> optionTexts;
	std::vector<MenuAction> optionActions;
	size_t selectedOption;
};
