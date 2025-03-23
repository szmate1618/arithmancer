#pragma once

#include <vector>
#include <iostream>

#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"
#include "input.hpp"


class Game {
public:
    enum class State {
        MENU,
        WANDERING,
        BATTLE
    };

    //Game(Map& gameMap) : map(gameMap), gameState(State::MENU), isRunning(true) {}
    Game() :
        map(100, 50, 1),
        menu({ "New game", "Quit" }, { [this]()->void { this->NewGame(); }, [this]()->void { this->Quit(); } }),
        gameState(State::MENU),
        isRunning(true)
    {
    }

    void Draw(ScreenBuffer& screenBuffer) {
        screenBuffer.clear();

        switch (gameState) {
        case State::MENU:
			menu.Draw(screenBuffer);
            break;
        case State::WANDERING:
            map.PrintDungeon(screenBuffer);
            break;
        case State::BATTLE:
            drawBattle();
            break;
        }
    }

	void Update() {
		InputHandler::Update();
		switch (gameState) {
		case State::MENU:
			menu.Update();
			break;
		case State::WANDERING:
            map.Update();
			break;
		case State::BATTLE:
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

		map = Map(100, 50, 20);
	}

private:
    Map map;
    Menu menu;
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
