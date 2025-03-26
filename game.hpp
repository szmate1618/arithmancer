#pragma once

#include <vector>
#include <iostream>

#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"
#include "input.hpp"
#include "battle.hpp"


class Game {
public:
	enum class State {
		MENU,
		WANDERING,
		ENTERING_BATTLE,
		BATTLE
	};

	//Game(Map& gameMap) : map(gameMap), gameState(State::MENU), isRunning(true) {}
	Game() :
		map([this]()->void { this->StartBattle(); }, 100, 50, 1),
		menu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
		battle([this]()->void { this->EndBattle(); }),
		gameState(State::MENU),
		isRunning(true)
	{
	}

	void Draw(ScreenBuffer& screenBuffer) {
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

	void Update(double seconds) {
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

	bool IsRunning() const {
		return isRunning;
	}

	void Quit() {
		isRunning = false;
	}

	void NewGame() {
		gameState = State::WANDERING;

		map = Map([this]()->void { this->StartBattle(); }, 100, 50, 20);
	}

	void EndBattle() {
		gameState = State::WANDERING;
	}

	void StartBattle() {
		gameState = State::ENTERING_BATTLE;

		battle = Battle([this]()->void { this->EndBattle(); });
		battle.AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
		battle.AddProblem(std::make_unique<AdditionProblem>(AdditionProblem()));
	}

private:
	Map map;
	Menu menu;
	Battle battle;
	State gameState;
	bool isRunning;

	void drawMenu() {
		std::cout << "=== MENU ===\n";
	}

	void drawWandering(const std::vector<std::vector<char>>& grid) {
		for (const auto& row : grid) {
			for (char cell : row) {
				std::cout << cell << ' ';
			}
			std::cout << '\n';
		}
	}

	void drawBattle() {
		std::cout << "=== BATTLE ===\n";
	}
};
