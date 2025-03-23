#pragma once

#include <vector>
#include <iostream>

#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"


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

    void Draw(const std::vector<std::vector<char>>& grid) {
        switch (gameState) {
        case State::MENU:
            drawMenu();
            break;
        case State::WANDERING:
            drawWandering(grid);
            break;
        case State::BATTLE:
            drawBattle();
            break;
        }
    }

	void Update() {
		switch (gameState) {
		case State::MENU:
			break;
		case State::WANDERING:
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
