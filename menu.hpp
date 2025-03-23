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

    Menu(const std::vector<std::string>& options, const std::vector<MenuAction>& actions);
    void Draw(ScreenBuffer& screenBuffer);
    void Update();

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

Menu::Menu(const std::vector<std::string>& options, const std::vector<MenuAction>& actions)
    : optionTexts(options), optionActions(actions), selectedOption(0) {
}

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

        if (i == selectedOption) {
            screenBuffer.setChar(startH, startW - 2, '>');
        }
    }
}

void Menu::Update()
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
