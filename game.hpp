#pragma once

#include <memory>

#include "map.hpp"
#include "screen.hpp"
#include "menu.hpp"
#include "input.hpp"
#include "battle.hpp"
#include "quest.hpp"


class Game {
public:
	enum class State {
		MENU,
		QUEST_RUNNING
	};

	Game();
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);
	bool IsRunning() const;
	void Quit();
	void NewGame();

private:
	Menu menu;
	std::unique_ptr<Quest> quest;
	State gameState;
	bool isRunning;
};
