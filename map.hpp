#pragma once

#include <functional>
#include <vector>

#include "enemy.hpp"
#include "screen.hpp"
#include "projectile.hpp"


class Map
{
public:
	enum class TileType { WALL, FLOOR, WATER, LAVA, START, GOAL, EMPTY, PLAYER, ENEMY, PROJECTILE };

	enum class Direction { EAST, NORTH_EAST, NORTH, NORTH_WEST, WEST, SOUTH_WEST, SOUTH, SOUTH_EAST };

	struct Camera {
		int x, y;
	};

	struct Entity {
		int x, y;
		TileType standingOn;
		bool isActive = true;
		Direction direction = Direction::EAST;

		// Coordinate mappings for direction. Keep it in sync with the Direction enum.
		static constexpr int dx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		static constexpr int dy[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	};

	struct EnemyEntity : public Entity {
		Enemy agent;
	};

	struct ProjectileEntity : public Entity {
		Projectile agent;
	};

	struct PlayerEntity :public Entity {
		int health = 10;
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

	bool HasPlayerWon() const;
	bool HasPlayerLost() const;

private:
	std::function<void()> startBattleCallback;
	size_t WIDTH, HEIGHT;
	std::vector<std::vector<TileType>> grid;
	std::vector<std::vector<bool>> fogOfWar;
	static constexpr char tileChars[] = { '#', ' ', '~', '~', 'S', 'G', '.', '@', '*', 'o'};
	static constexpr bool walkable[] = { false, true, false, false, true, true, true, true, true, true };
	static constexpr bool walkableByEnemy[] = { false, true,  false, false, true, true, true, true, false, true };
	Camera camera;
	PlayerEntity player;
	std::vector<EnemyEntity> enemies;
	std::vector<ProjectileEntity> projectiles;

	struct Room {
		int x, y, w, h;
	};

	enum class WIN_CONDITION { NONE, REACHED_GOAL, DEFEATED_ALL_ENEMIES, DEFEATED_ALL_ENEMIES_AND_REACHED_GOAL };
	WIN_CONDITION winCondition = WIN_CONDITION::NONE;
	enum class LOSE_CONDITION { NONE, LOST_ALL_HEALTH };
	LOSE_CONDITION loseCondition = LOSE_CONDITION::NONE;

	void Reset();

	void UpdateEnemies(double seconds);
	void RemoveInactiveEnemies();
	void RemoveInactiveProjectiles();
	void UpdateProjectiles(double seconds);
	void SpawnProjectile(int x, int y, Direction direction);

	void RevealArea();
	bool HasLineOfSight(int x1, int y1, int x2, int y2) const;
	size_t ManhattanDistance(int x1, int y1, int x2, int y2) const;
};
