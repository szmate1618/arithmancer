#include <vector>
#include <iostream>

#include "input.hpp"

#include "game.hpp"


Game::Game() :
	menu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
	gameState(State::MENU),
	campaigns(),
	isRunning(true)
{
	campaigns.push_back(std::make_unique<Campaign>());
	campaigns.back()->AddQuest(std::make_unique<Quest>(*this));
}

void Game::Draw(ScreenBuffer& screenBuffer) {
	switch (gameState) {
	case State::MENU:
		menu.Draw(screenBuffer);
		break;
	case State::CAMPAIGN_RUNNING:
		screenBuffer.clear();
		campaigns.at(currentCampaignIndex)->Draw(screenBuffer);
		break;
	}
}

void Game::Update(double seconds) {
	InputHandler::Update();
	switch (gameState) {
	case State::MENU:
		menu.Update(seconds);
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
	gameState = State::CAMPAIGN_RUNNING;

	currentCampaignIndex = 0;
}
