#pragma once

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

	Game();
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);
	bool IsRunning() const;
	void Quit();
	void NewGame();
	void EndBattle();
	void StartBattle();

private:
	Map map;
	Menu menu;
	Battle battle;
	State gameState;
	bool isRunning;
};
