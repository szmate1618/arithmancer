#pragma once

#include <vector>
#include <memory>

#include "quest.hpp"
#include "screen.hpp"


class Campaign {
public:
	void AddQuest(std::unique_ptr<Quest> quest);

	void Update(double seconds);

	void Draw(ScreenBuffer& screenBuffer) const;

private:
	std::vector<std::unique_ptr<Quest>> quests;
	size_t currentQuestIndex = 0;
};
