#pragma once

#include <memory>

#include "screen.hpp"
#include "map.hpp"
#include "battle.hpp"


class Game;

class Quest
{
public:
	enum class State {
		WANDERING,
		ENTERING_BATTLE,
		BATTLE
	};

	Quest(Game& game);
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);
private:
	State state;
	Game& game;
	std::unique_ptr<Map> map;
	std::unique_ptr<Battle> battle;

	void EndBattle();
	void StartBattle();
};
