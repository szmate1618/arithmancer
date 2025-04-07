#pragma once

#include <memory>

#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"
#include "input.hpp"
#include "battle.hpp"
#include "campaign.hpp"


class Game {
public:
	enum class State {
		MAIN_MENU,
		CAMPAIGN_SELECTION_MENU,
		QUEST_SELECTION_MENU,
		CAMPAIGN_RUNNING
	};

	Game();
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);
	bool IsRunning() const;
	void Quit();
	void NewGame();

private:
	Menu mainMenu;
	Menu campaignSelectionMenu;
	Menu questSelectionMenu;
	std::vector<std::unique_ptr<Campaign>> campaigns;
	size_t currentCampaignIndex = 0;
	State gameState;
	bool isRunning;
};
