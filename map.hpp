#pragma once

#include <functional>
#include <vector>

#include "enemy.hpp"
#include "screen.hpp"


class Map
{
public:
	enum class TileType { WALL, FLOOR, START, GOAL, EMPTY, PLAYER, ENEMY };

	struct Entity {
		int x, y;
		TileType standingOn;
	};
	struct EnemyEntity : public Entity {
		Enemy agent;
	};

	Map(const std::function<void()>& startBattleCallback, size_t width, size_t height, size_t roomCount);
	void PrintDungeon(ScreenBuffer& screenBuffer);
	bool IsWalkable(int x, int y) const;
	bool IsWalkableByEnemy(int x, int y) const;
	void PlacePlayer(int x, int y);
	void Update(double seconds);
	std::vector<std::pair<int, int>> GetShortestPath(int startX, int startY, int goalX, int goalY) const;

	// After this function is called, the Map object cannot be copied,
	// because the enemies are initialized with callbacks to member functions through the this pointer.
	void GenerateDungeon(size_t roomCount);
	void LoadFromString(const std::string& mapString);

private:
	std::function<void()> startBattleCallback;
	size_t WIDTH, HEIGHT;
	std::vector<std::vector<TileType>> grid;
	std::vector<std::vector<bool>> fogOfWar;
	static constexpr char tileChars[] = { '#', ' ', 'S', 'G', '.', '@', '*' };
	static constexpr bool walkable[] = { false, true, true, true, true, true, true };
	static constexpr bool walkableByEnemy[] = { false, true, true, true, true, true, false };
	Entity player;
	std::vector<EnemyEntity> enemies;

	struct Room {
		int x, y, w, h;
	};

	void Reset();

	void UpdateEnemies(double seconds);

	void RevealArea();
	bool HasLineOfSight(int x1, int y1, int x2, int y2) const;
	size_t ManhattanDistance(int x1, int y1, int x2, int y2) const;
};
