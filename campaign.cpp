#pragma once

#include "screen.hpp"

#include "campaign.hpp"


void Campaign::AddQuest(std::unique_ptr<Quest> quest) {
	quests.push_back(std::move(quest));

	// Reconstruct the quest selection menu.
	std::vector<std::string> options;
	std::vector<Menu::MenuAction> actions;

	for (size_t i = 0; i < quests.size(); i++)
	{
		options.push_back(std::format("Quest {0}", i));
		actions.push_back(
			[this, i]() {  // Important to capture i by value.
				this->currentQuestIndex = i;
				this->state = State::RUNNING;
			}
		);
	}

	questSelectionMenu = Menu(options, actions);
}

void Campaign::Update(double seconds) {
	switch (state) {
	case State::QUEST_SELECTION_MENU:
		questSelectionMenu.Update(seconds);
		break;
	case State::RUNNING:
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

		break;
	}
}

void Campaign::Draw(ScreenBuffer& screenBuffer) const {
	switch (state) {
	case State::QUEST_SELECTION_MENU:
		questSelectionMenu.Draw(screenBuffer);
		break;
	case State::RUNNING:
		if (currentQuestIndex < quests.size()) {
			quests[currentQuestIndex]->Draw(screenBuffer);
		}
		break;
	}
}
