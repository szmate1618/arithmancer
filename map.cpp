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
#include <sstream>

#include "input.hpp"
#include "screen.hpp"

#include "map.hpp"


Map::Map(const std::function<void()>& startBattleCallback, size_t width, size_t height, size_t roomCount) :
	startBattleCallback(startBattleCallback),
	WIDTH(width),
	HEIGHT(height),
	grid(height, std::vector<TileType>(width, TileType::WALL)),
	fogOfWar(height, std::vector<bool>(width, true))
{
}

void Map::PrintDungeon(ScreenBuffer& screenBuffer) {
	size_t cameraDiffX = -camera.x + screenBuffer.getWidth() / 2;
	size_t cameraDiffY = -camera.y + screenBuffer.getHeight() / 2;

	for (size_t i = 0; i < screenBuffer.getHeight(); i++) {
		for (size_t j = 0; j < screenBuffer.getWidth(); j++) {
			size_t i2 = i - cameraDiffY;
			size_t j2 = j - cameraDiffX;
			if (i2 >= 0 && i2 < grid.size() && j2 >= 0 && j2 < grid.at(i2).size()) {
				if (fogOfWar[i2][j2] && (grid[i2][j2] == TileType::FLOOR || grid[i2][j2] == TileType::EMPTY || grid[i2][j2] == TileType::ENEMY)) {
					screenBuffer.setChar(i, j, '.');
				}
				else {
					screenBuffer.setChar(i, j, tileChars[static_cast<int>(grid.at(i2).at(j2))]);
				}
			}
		}
	}

	for (const ProjectileEntity& projectile : projectiles) {
		size_t x2 = projectile.x + cameraDiffX;
		size_t y2 = projectile.y + cameraDiffY;
		if (x2 >= 0 && x2 < screenBuffer.getWidth() && y2 >= 0 && y2 < screenBuffer.getHeight()) {
			screenBuffer.setChar(y2, x2, tileChars[static_cast<int>(TileType::PROJECTILE)]);
		}
	}

	size_t x2 = player.x + cameraDiffX;
	size_t y2 = player.y + cameraDiffY;
	if (x2 >= 0 && x2 < screenBuffer.getWidth() && y2 >= 0 && y2 < screenBuffer.getHeight()) {
		screenBuffer.setChar(y2, x2, tileChars[static_cast<int>(TileType::PLAYER)]);
	}
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

		int dx = x - player.x;
		int dy = y - player.y;
		for (size_t i = 0; i < 8; i++)
		{
			if (Entity::dx[i] == dx && Entity::dy[i] == dy) {
				player.direction = static_cast<Direction>(i);
				break;
			}
		}

		player.x = x;
		player.y = y;
		player.standingOn = grid[y][x];
		RevealArea();

		const int cameraLag = 15;
		if (player.x - camera.x > cameraLag) camera.x = player.x - cameraLag;
		if (camera.x - player.x > cameraLag) camera.x = player.x + cameraLag;
		if (player.y - camera.y > cameraLag) camera.y = player.y - cameraLag;
		if (camera.y - player.y > cameraLag) camera.y = player.y + cameraLag;
	}
}

void Map::Update(double seconds)
{
	RemoveInactiveEnemies();
	RemoveInactiveProjectiles();

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
	}
	if (InputHandler::IsPressed(' ')) {
		SpawnProjectile(player.x, player.y, player.direction);
	}

	UpdateProjectiles(seconds);
	UpdateEnemies(seconds);

	for (ProjectileEntity& projectile : projectiles) {
		int x = projectile.agent.GetPosition().first;
		int y = projectile.agent.GetPosition().second;
		if (IsWalkable(x, y)) {
			projectile.x = x;
			projectile.y = y;
			projectile.standingOn = grid[y][x];
		}
		else {
			projectile.isActive = false;
		}
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
			projectile.isActive = false;
		}

		if (projectile.isActive == false) continue;
		for (EnemyEntity& enemy : enemies) {
			if (enemy.isActive == false) continue;
			if (enemy.x == projectile.x && projectile.y == player.y) {
				// Mark the enemy as inactive, so that it will be removed after the battle.
				projectile.isActive = false;
				startBattleCallback();
				break;  // TODO: Figure out if this really is needed.
			}
		}
	}

	for (EnemyEntity& enemy : enemies) {
		if (enemy.isActive == false) continue;
		if (enemy.x == player.x && enemy.y == player.y) {
			// Mark the enemy as inactive, so that it will be removed after the battle.
			enemy.isActive = false;
			startBattleCallback();
			break;  // TODO: Figure out if this really is needed.
		}
	}
}

void Map::Reset()
{
	enemies = {};
}

void Map::UpdateEnemies(double seconds)
{
	for (EnemyEntity& enemy : enemies) {
		enemy.agent.Update(seconds);
		std::pair<int, int> pos = enemy.agent.GetPosition();
		if (pos.first != enemy.x || pos.second != enemy.y) {
			// TODO: Move this to a separate function.
			grid[enemy.y][enemy.x] = enemy.standingOn;
			enemy.x = pos.first;
			enemy.y = pos.second;
			enemy.standingOn = grid[enemy.y][enemy.x];
			grid[enemy.y][enemy.x] = TileType::ENEMY;
		}

		if (ManhattanDistance(player.x, player.y, enemy.x, enemy.y) <= 15 &&
			HasLineOfSight(player.x, player.y, enemy.x, enemy.y))
		{
			enemy.agent.SpotPlayer(player.x, player.y);
			enemy.agent.AppendPath(GetShortestPath(enemy.x, enemy.y, player.x, player.y));
		}
	}
}

void Map::UpdateProjectiles(double seconds) {
	for (ProjectileEntity& projectile: projectiles) {
		projectile.agent.Update(seconds);
	}
}

void Map::RemoveInactiveEnemies() {
	for (EnemyEntity& enemy : enemies) {
		if (!enemy.isActive) {
			grid[enemy.y][enemy.x] = enemy.standingOn;
		}
	}
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const EnemyEntity& enemy) { return !enemy.isActive; }), enemies.end());
}

void Map::RemoveInactiveProjectiles() {
	for (ProjectileEntity& projectile : projectiles) {
		if (!projectile.isActive) {
			grid[projectile.y][projectile.x] = projectile.standingOn;
		}
	}
	projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const ProjectileEntity& projectile) { return !projectile.isActive; }), projectiles.end());
}

void Map::SpawnProjectile(int x, int y, Direction direction) {
	projectiles.push_back({ x, y, TileType::EMPTY, true, direction,
		Projectile(x, y, Entity::dx[static_cast<int>(direction)], Entity::dy[static_cast<int>(direction)])
	});
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
	Reset();

	std::vector<Room> rooms;
	srand(time(0));

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
					enemies.push_back({ x, y, TileType::FLOOR, true, Direction::EAST,
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
			path.emplace_back(startX, startY);
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

void Map::LoadFromString(const std::string& mapString) {
	Reset();

	std::istringstream stream(mapString);
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(stream, line)) {
		lines.push_back(line);
	}
	HEIGHT = lines.size();
	WIDTH = lines.empty() ? 0 : lines[0].size();
	grid = std::vector<std::vector<TileType>>(HEIGHT, std::vector<TileType>(WIDTH, TileType::WALL));
	fogOfWar = std::vector<std::vector<bool>>(HEIGHT, std::vector<bool>(WIDTH, true));

	for (int y = 0; y < static_cast<int>(HEIGHT); ++y) {
		for (int x = 0; x < static_cast<int>(WIDTH); ++x) {
			if (x < static_cast<int>(lines[y].size())) {
				auto it = std::find(std::begin(tileChars), std::end(tileChars), lines[y][x]);
				if (it != std::end(tileChars)) {
					TileType tileType = static_cast<TileType>(std::distance(std::begin(tileChars), it));
					grid[y][x] = tileType;
					if (tileType == TileType::START || tileType == TileType::PLAYER) {
						PlacePlayer(x, y);
					}
					if (tileType == TileType::ENEMY) {
						enemies.push_back({ x, y, TileType::FLOOR, true, Direction::EAST,
							Enemy([this](int x, int y)->bool { return this->IsWalkableByEnemy(x, y); }, x, y)
							});
					}
				}
			}
		}
	}
}

bool Map::HasPlayerWon() const
{
	switch (winCondition) {
	case WIN_CONDITION::NONE:
		return false;
	case WIN_CONDITION::REACHED_GOAL:
		return grid.at(player.y).at(player.x) == TileType::GOAL;
	case WIN_CONDITION::DEFEATED_ALL_ENEMIES:
		return enemies.size() == 0;
	case WIN_CONDITION::DEFEATED_ALL_ENEMIES_AND_REACHED_GOAL:
		return enemies.size() == 0 && grid.at(player.y).at(player.x) == TileType::GOAL;
	default:
		return true;
	}
}

bool Map::HasPlayerLost() const
{
	switch (loseCondition) {
	case LOSE_CONDITION::NONE:
		return false;
	case LOSE_CONDITION::LOST_ALL_HEALTH:
		return player.health <= 0;
	default:
		return true;
	}
}
