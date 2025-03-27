#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <queue>
#include <map>
#include <cmath>

#include "screen.hpp"
#include "input.hpp"
#include "enemy.hpp"


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

	void UpdateEnemies(double seconds);

	void RevealArea();
	bool HasLineOfSight(int x1, int y1, int x2, int y2) const;
	size_t ManhattanDistance(int x1, int y1, int x2, int y2) const;
};

Map::Map(const std::function<void()>& startBattleCallback, size_t width, size_t height, size_t roomCount) :
	startBattleCallback(startBattleCallback),
	WIDTH(width),
	HEIGHT(height),
	grid(height, std::vector<TileType>(width, TileType::WALL)),
	fogOfWar(height, std::vector<bool>(width, true))
{
}

void Map::PrintDungeon(ScreenBuffer& screenBuffer) {
	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid.at(i).size(); j++) {
			if (fogOfWar[i][j] && (grid[i][j] == TileType::FLOOR || grid[i][j] == TileType::EMPTY || grid[i][j] == TileType::ENEMY)) {
				screenBuffer.setChar(i, j, '.');
			}
			else {
				screenBuffer.setChar(i, j, tileChars[static_cast<int>(grid.at(i).at(j))]);
			}
		}
	}
	screenBuffer.setChar(player.y, player.x, tileChars[static_cast<int>(TileType::PLAYER)]);
}

bool Map::IsWalkable(int x, int y) const {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
	return walkable[static_cast<int>(grid[y][x])];
}

bool Map::IsWalkableByEnemy(int x, int y) const {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
	return walkableByEnemy[static_cast<int>(grid[y][x])];
}

void Map::PlacePlayer(int x, int y) {
	if (IsWalkable(x, y)) {
		player.x = x;
		player.y = y;
		player.standingOn = grid[y][x];
		RevealArea();
	}
}

void Map::Update(double seconds)
{
	int dx = 0, dy = 0;
	if (InputHandler::IsUpPressed()) dy = -1;
	if (InputHandler::IsDownPressed()) dy = 1;
	if (InputHandler::IsLeftPressed()) dx = -1;
	if (InputHandler::IsRightPressed()) dx = 1;
	if (dx != 0 || dy != 0) {
		int newX = player.x + dx;
		int newY = player.y + dy;
		if (IsWalkable(newX, newY)) {
			PlacePlayer(newX, newY);
		}
		else if (IsWalkable(newX, player.y)) {
			PlacePlayer(newX, player.y);
		}
		else if (IsWalkable(player.x, newY)) {
			PlacePlayer(player.x, newY);
		}
		if (player.standingOn == TileType::ENEMY) {
			startBattleCallback();
		}
	}

	UpdateEnemies(seconds);
}

void Map::UpdateEnemies(double seconds)
{
	for (EnemyEntity& enemy : enemies) {
		enemy.agent.Update(seconds);

		if (ManhattanDistance(player.x, player.y, enemy.x, enemy.y) <= 15 &&
			HasLineOfSight(player.x, player.y, enemy.x, enemy.y))
		{
			enemy.agent.SpotPlayer(player.x, player.y);
			enemy.agent.SetPath(GetShortestPath(enemy.x, enemy.y, player.x, player.y));
		}
	}
}

void Map::RevealArea() {
	int radius = 5;
	for (int dy = -radius; dy <= radius; ++dy) {
		for (int dx = -radius; dx <= radius; ++dx) {
			int nx = player.x + dx;
			int ny = player.y + dy;
			if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
				if (HasLineOfSight(player.x, player.y, nx, ny)) {
					fogOfWar[ny][nx] = false;
				}
			}
		}
	}
}

bool Map::HasLineOfSight(int x1, int y1, int x2, int y2) const {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
	while (x1 != x2 || y1 != y2) {
		if (grid[y1][x1] == TileType::WALL) return false;
		int e2 = err * 2;
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}
	return true;
}

size_t Map::ManhattanDistance(int x1, int y1, int x2, int y2) const {
	return abs(x2 - x1) + abs(y2 - y1);
}

// Dungeon generation function
void Map::GenerateDungeon(size_t roomCount) {
	std::vector<Room> rooms;
	srand(time(0));

	enemies = {};

	// Place rooms
	for (int i = 0; i < roomCount; ++i) {
		int rw = 5 + rand() % 10; // Room width: 5-14
		int rh = 5 + rand() % 10; // Room height: 5-14
		int rx = rand() % (WIDTH - rw - 1) + 1;
		int ry = rand() % (HEIGHT - rh - 1) + 1;

		// Create room
		Room room = { rx, ry, rw, rh };
		rooms.push_back(room);

		// Carve out the room
		for (int y = ry; y < ry + rh; ++y)
			for (int x = rx; x < rx + rw; ++x)
				grid[y][x] = TileType::FLOOR;
	}

	// Connect rooms with corridors
	for (size_t i = 1; i < rooms.size(); ++i) {
		Room a = rooms[i - 1];
		Room b = rooms[i];

		int x1 = a.x + a.w / 2;
		int y1 = a.y + a.h / 2;
		int x2 = b.x + b.w / 2;
		int y2 = b.y + b.h / 2;

		// Horizontal tunnel
		for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
			grid[y1][x] = TileType::FLOOR;

		// Vertical tunnel
		for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
			grid[y][x2] = TileType::FLOOR;
	}

	// Pass 1: Remove isolated walls (turn WALL into EMPTY if not adjacent to FLOOR)
	std::vector<std::vector<TileType>> tempGrid = grid;
	for (int y = 1; y < HEIGHT - 1; ++y) {
		for (int x = 1; x < WIDTH - 1; ++x) {
			if (grid[y][x] == TileType::WALL) {
				bool hasNeighbor = false;
				for (int dy = -1; dy <= 1; ++dy)
					for (int dx = -1; dx <= 1; ++dx)
						if (grid[y + dy][x + dx] == TileType::FLOOR)
							hasNeighbor = true;

				if (!hasNeighbor)
					tempGrid[y][x] = TileType::EMPTY; // Remove isolated walls
			}
		}
	}
	grid = tempGrid;

	// Pass 2: Add enemies.
	for (int y = 0; y < HEIGHT; ++y)
		for (int x = 0; x < WIDTH; ++x)
			if (grid[y][x] == TileType::FLOOR)
				if (rand() % 100 == 0) {
					grid[y][x] = TileType::ENEMY;
					enemies.push_back({ x, y, TileType::FLOOR,
						Enemy([this](int x, int y)->bool { return this->IsWalkableByEnemy(x, y); }, x, y)
						});
				}

	// Set START and GOAL positions
	if (!rooms.empty()) {
		int sx = rooms.front().x + rooms.front().w / 2;
		int sy = rooms.front().y + rooms.front().h / 2;
		grid[sy][sx] = TileType::START;
		PlacePlayer(sx, sy);

		int gx = rooms.back().x + rooms.back().w / 2;
		int gy = rooms.back().y + rooms.back().h / 2;
		grid[gy][gx] = TileType::GOAL;
	}
}

std::vector<std::pair<int, int>> Map::GetShortestPath(int startX, int startY, int goalX, int goalY) const {
	struct Node {
		int x, y, cost, heuristic;
		bool operator>(const Node& other) const { return (cost + heuristic) > (other.cost + other.heuristic); }
	};

	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
	std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
	std::map<std::pair<int, int>, int> costSoFar;

	auto heuristic = [](int x1, int y1, int x2, int y2) {
		return abs(x2 - x1) + abs(y2 - y1);
		};

	openSet.push({ startX, startY, 0, heuristic(startX, startY, goalX, goalY) });
	costSoFar[{startX, startY}] = 0;

	while (!openSet.empty()) {
		Node current = openSet.top();
		openSet.pop();

		if (current.x == goalX && current.y == goalY) {
			std::vector<std::pair<int, int>> path;
			while (cameFrom.count({ current.x, current.y })) {
				path.emplace_back(current.x, current.y);
				current = { cameFrom[{current.x, current.y}].first, cameFrom[{current.x, current.y}].second, 0, 0 };
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		for (int dy = -1; dy <= 1; ++dy) {
			for (int dx = -1; dx <= 1; ++dx) {
				if (dx == 0 && dy == 0) continue;
				int nx = current.x + dx;
				int ny = current.y + dy;
				if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
				if (!IsWalkable(nx, ny)) continue;
				int newCost = costSoFar[{current.x, current.y}] + 1;
				if (!costSoFar.count({ nx, ny }) || newCost < costSoFar[{nx, ny}]) {
					costSoFar[{nx, ny}] = newCost;
					int priority = newCost + heuristic(nx, ny, goalX, goalY);
					openSet.push({ nx, ny, newCost, priority });
					cameFrom[{nx, ny}] = { current.x, current.y };
				}
			}
		}
	}
	return {};
}
