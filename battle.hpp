#pragma once

#include <vector>
#include <memory>
#include <format>
#include <functional>

#include "problem.hpp"
#include "screen.hpp"
#include "textfield.hpp"
#include "input.hpp"


class Battle {
private:
	std::function<void()> endBattleCallback;

    std::vector<std::unique_ptr<Problem>> problems;
    std::vector<TextField> inputFields;
	size_t currentProblemIndex;

	bool HasEnded()
	{
		for (size_t i = 0; i < problems.size(); i++)
		{
			if (!problems[i]->HasTimedOut() && !problems[i]->IsSolved())
			{
				return false;
			}
		}
		return true;
	}

public:
	Battle(std::function<void()> endBattleCallback) : endBattleCallback(endBattleCallback), currentProblemIndex(0) {}

    void AddProblem(std::unique_ptr<Problem> problem) {
        problems.push_back(std::move(problem));
		inputFields.push_back(TextField());
    }

    void Update(double seconds) {
		if (InputHandler::IsUpPressed() || InputHandler::IsPressed('w'))
		{
			currentProblemIndex = (currentProblemIndex - 1) % inputFields.size();
		}
		if (InputHandler::IsDownPressed() || InputHandler::IsPressed('s'))
		{
			currentProblemIndex = (currentProblemIndex + 1) % inputFields.size();
		}

		for (size_t i = 0; i < problems.size(); i++)
		{
			if (!problems[i]->IsSolved())
			{
				problems[i]->Update(seconds);
				if (i == currentProblemIndex)
				{
					inputFields[i].Update(seconds);

					if (InputHandler::IsPressed(VK_RETURN))
					{
						problems[i]->Guess(inputFields[i].GetText());
						if (problems[i]->IsSolved())
						{
							currentProblemIndex = (currentProblemIndex + 1) % inputFields.size();
						}
					}
				}
			}
		}

		if (HasEnded())
		{
			endBattleCallback();
		}
    }

	void Draw(ScreenBuffer& screenBuffer) {
		
		size_t margin = 3;
		size_t gap = 1;
		size_t problemTextHeight = 3;
		size_t height = problems.size() * (problemTextHeight + gap) + gap;

		size_t firstLine = screenBuffer.getHeight() - height;

		for (size_t i = firstLine; i < screenBuffer.getHeight(); i++)
		{
			for (size_t j = 0; j < screenBuffer.getWidth(); j++)
			{
				if (i == firstLine) screenBuffer.setChar(i, j, '-');
				else screenBuffer.setChar(i, j, ' ');
			}
		}

		size_t currentLine = firstLine + gap;
		for (size_t i = 0; i < problems.size(); i++) {
			std::string problemHeader = std::format("{0} - error margin = {1}", problems[i]->GetName(), problems[i]->GetErrorMargin());
			std::string problemStatement = problems[i]->GetProblemStatement() + std::string(" = ");
			std::string remainingTime = "Remaining time: ";
			std::string timeBar = remainingTime;
			size_t barMaxLength = screenBuffer.getWidth() - margin * 2 - remainingTime.length();
			size_t barLength = static_cast<int>(problems[i]->GetRemainingTime() / problems[i]->GetTimeLimit() * barMaxLength);
			if (barLength > 0) timeBar += std::string(barLength, '=');

			for (size_t j = 0; j < problemHeader.length(); j++)
			{
				screenBuffer.setChar(currentLine, margin + j, problemHeader[j]);
			}
			currentLine++;

			if (i == currentProblemIndex)
			{
				screenBuffer.setChar(currentLine, margin - 2, '>');
			}
			for (size_t j = 0; j < problemStatement.length(); j++)
			{
				screenBuffer.setChar(currentLine, margin + j, problemStatement[j]);
			}
			for (size_t j = 0; j < inputFields[i].GetText().length(); j++)
			{
				screenBuffer.setChar(currentLine, margin + j + problemStatement.length(), inputFields[i].GetText()[j]);
			}
			currentLine++;

			for (size_t j = 0; j < timeBar.length(); j++)
			{
				screenBuffer.setChar(currentLine, margin + j, timeBar[j]);
			}
			currentLine++;
			currentLine += gap;
		}
	}
};
