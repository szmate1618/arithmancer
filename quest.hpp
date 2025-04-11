#pragma once

#include <memory>

#include "screen.hpp"
#include "map.hpp"
#include "battle.hpp"


class Game;

struct QuestConfiguration
{
	enum class MapSource { BUILTIN, FROM_FILE, RANDOM_GENERATED } mapSource;
	int arg1;
};

class Quest
{
public:
	enum class State {
		WANDERING,
		ENTERING_BATTLE,
		BATTLE
	};

	Quest(Game& game);
	Quest(Game& game, QuestConfiguration configuration);
	void Draw(ScreenBuffer& screenBuffer);
	void Update(double seconds);

	bool HasPlayerWon() const;
	bool HasPlayerLost() const;
private:
	State state;
	Game& game;
	std::unique_ptr<Map> map;
	std::unique_ptr<Battle> battle;

	void EndBattle();
	void StartBattle();
};
