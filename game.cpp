#include <vector>
#include <iostream>

#include "input.hpp"

#include "game.hpp"


Game::Game() :
	menu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
	gameState(State::MENU),
	quest(nullptr),
	isRunning(true)
{
}

void Game::Draw(ScreenBuffer& screenBuffer) {
	switch (gameState) {
	case State::MENU:
		menu.Draw(screenBuffer);
		break;
	case State::QUEST_RUNNING:
		quest->Draw(screenBuffer);
		break;
	}
}

void Game::Update(double seconds) {
	InputHandler::Update();
	switch (gameState) {
	case State::MENU:
		menu.Update(seconds);
		break;
	case State::QUEST_RUNNING:
		quest->Update(seconds);
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
	gameState = State::QUEST_RUNNING;

	quest = std::make_unique<Quest>(*this);
}
