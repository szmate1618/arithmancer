#pragma once

#include "screen.hpp"

#include "campaign.hpp"


void Campaign::AddQuest(std::unique_ptr<Quest> quest) {
	quests.push_back(std::move(quest));
}

void Campaign::Update(double seconds) {
	if (currentQuestIndex >= quests.size()) {
		std::cout << "All quests completed!\n";
		return;
	}

	Quest* currentQuest = quests[currentQuestIndex].get();
	currentQuest->Update(seconds);

	if (currentQuest->HasPlayerWon()) {
		std::cout << "Quest " << currentQuestIndex << " completed!\n";
		++currentQuestIndex;
	}
}

void Campaign::Draw(ScreenBuffer& screenBuffer) const {
	if (currentQuestIndex < quests.size()) {
		quests[currentQuestIndex]->Draw(screenBuffer);
	}
}
