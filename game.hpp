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
		MENU,
		CAMPAIGN_RUNNING
	};

	Game();
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);
	bool IsRunning() const;
	void Quit();
	void NewGame();

private:
	Menu menu;
	std::vector<std::unique_ptr<Campaign>> campaigns;
	size_t currentCampaignIndex = 0;
	State gameState;
	bool isRunning;
};
