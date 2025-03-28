#include <vector>
#include <iostream>

#include "input.hpp"

#include "game.hpp"


Game::Game() :
	map([this]()->void { this->StartBattle(); }, 100, 50, 1),
	menu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
	battle([this]()->void { this->EndBattle(); }),
	gameState(State::MENU),
	isRunning(true)
{
}

void Game::Draw(ScreenBuffer& screenBuffer) {
	switch (gameState) {
	case State::MENU:
		menu.Draw(screenBuffer);
		break;
	case State::WANDERING:
		map.PrintDungeon(screenBuffer);
		break;
	case State::ENTERING_BATTLE:
		map.PrintDungeon(screenBuffer);
		battle.Draw(screenBuffer);
		break;
	case State::BATTLE:
		battle.Draw(screenBuffer);
		break;
	}
}

void Game::Update(double seconds) {
	InputHandler::Update();
	switch (gameState) {
	case State::MENU:
		menu.Update(seconds);
		break;
	case State::WANDERING:
		map.Update(seconds);
		break;
	case State::ENTERING_BATTLE:
		gameState = State::BATTLE;
		battle.Update(seconds);
		break;
	case State::BATTLE:
		battle.Update(seconds);
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
	gameState = State::WANDERING;

	map = Map([this]()->void { this->StartBattle(); }, 100, 50, 20);
	map.GenerateDungeon(20);
}

void Game::EndBattle() {
	gameState = State::WANDERING;
}

void Game::StartBattle() {
	gameState = State::ENTERING_BATTLE;

	battle = Battle([this]()->void { this->EndBattle(); });
	battle.AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
	battle.AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
}
