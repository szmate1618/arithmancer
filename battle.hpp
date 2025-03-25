#pragma once

#include <vector>
#include <memory>
#include <format>

#include "problem.hpp"
#include "screen.hpp"


class Battle {
private:
    std::vector<std::unique_ptr<Problem>> problems;

public:
    void AddProblem(std::unique_ptr<Problem> problem) {
        problems.push_back(std::move(problem));
    }

    void Update(double seconds) {
        for (auto& problem : problems) {
            problem->Update(seconds);
        }
    }

	void Draw(ScreenBuffer& screenBuffer) {
		
		size_t margin = 1;
		size_t gap = 1;
		size_t problemTextHeight = 3;
		size_t height = problems.size() * (problemTextHeight + gap) + margin;

		size_t firstLine = screenBuffer.getHeight() - 1 - height;

		for (size_t i = firstLine; i < screenBuffer.getHeight(); i++)
		{
			for (size_t j = 0; j < screenBuffer.getWidth(); j++)
			{
				if (i == firstLine) screenBuffer.setChar(i, j, '-');
				else screenBuffer.setChar(i, j, ' ');
			}
		}

		size_t currentLine = firstLine + margin;
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

			for (size_t j = 0; j < problemStatement.length(); j++)
			{
				screenBuffer.setChar(currentLine, margin + j, problemStatement[j]);
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
