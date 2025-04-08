#include <vector>
#include <iostream>

#include "input.hpp"

#include "game.hpp"


Game::Game() :
	mainMenu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
	campaignSelectionMenu(),
	gameState(State::MAIN_MENU),
	campaigns(),
	isRunning(true)
{
	campaigns.push_back(std::make_unique<Campaign>());
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this, QuestConfiguration(QuestConfiguration::MapSource::BUILTIN, 0)));
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this, QuestConfiguration(QuestConfiguration::MapSource::BUILTIN, 1)));
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this, QuestConfiguration(QuestConfiguration::MapSource::BUILTIN, 2)));
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this, QuestConfiguration(QuestConfiguration::MapSource::BUILTIN, 3)));
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this, QuestConfiguration(QuestConfiguration::MapSource::BUILTIN, 4)));

	campaignSelectionMenu = Menu(
		{ "Dev Campaign", "Campaign 2", "Campaign 3" },
		{
			[this]() { this->currentCampaignIndex = 0; this->gameState = State::CAMPAIGN_RUNNING; },
			[this]() { this->currentCampaignIndex = 1; this->gameState = State::CAMPAIGN_RUNNING; },
			[this]() { this->currentCampaignIndex = 2; this->gameState = State::CAMPAIGN_RUNNING; }
		}
	);
}

void Game::Draw(ScreenBuffer& screenBuffer) {
	switch (gameState) {
	case State::MAIN_MENU:
		mainMenu.Draw(screenBuffer);
		break;
	case State::CAMPAIGN_SELECTION_MENU:
		campaignSelectionMenu.Draw(screenBuffer);
		break;
	case State::CAMPAIGN_RUNNING:
		campaigns.at(currentCampaignIndex)->Draw(screenBuffer);
		break;
	}
}

void Game::Update(double seconds) {
	InputHandler::Update();
	switch (gameState) {
	case State::MAIN_MENU:
		mainMenu.Update(seconds);
		break;
	case State::CAMPAIGN_SELECTION_MENU:
		campaignSelectionMenu.Update(seconds);
		break;
	case State::CAMPAIGN_RUNNING:
		campaigns.at(currentCampaignIndex)->Update(seconds);
		break;
	}
}

bool Game::IsRunning() const {
	return isRunning;
}

void Game::Quit() {
	isRunning = false;
}

void Game::NewGame() {
	gameState = State::CAMPAIGN_SELECTION_MENU;

	currentCampaignIndex = 0;
}
