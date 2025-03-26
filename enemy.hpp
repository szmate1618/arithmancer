#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <functional>


class Enemy {
public:
	enum class State { GUARD, CHASE, PATROL };

	Enemy(std::function<bool(int, int)> isWalkableCallback, int x, int y) :
		isWalkableCallback(isWalkableCallback),
		posX(x), posY(y),
		guardX(x), guardY(y),
		targetX(-1), targetY(-1),
		state(State::GUARD),
		playerLastSpotted(5.0), moveTimer(0.0), timeToMove(1.0),
		pathIndex(0)
	{
	}

	void SpotPlayer(int playerX, int playerY) {
		targetX = playerX;
		targetY = playerY;
		playerLastSpotted = 0.0;
		state = State::CHASE;
	}

	void Update(double seconds) {
		playerLastSpotted += seconds;
		moveTimer += seconds;

		if (playerLastSpotted > 5.0) {
			state = State::GUARD;
		}
	}

	void Move()
	{
		if (moveTimer >= timeToMove) {
			moveTimer = 0.0;
			switch (state) {
			case State::CHASE:
				MoveAlongPath();
				break;
			case State::GUARD:
				MoveToGuardPosition();
				break;
			case State::PATROL:
				break;
			}
		}
	}

	void SetPath(const std::vector<std::pair<int, int>>& newPath) {
		path = newPath;
		pathIndex = 0;
	}

private:
	std::function<bool(int, int)> isWalkableCallback;
	int posX, posY;
	int targetX, targetY;
	int guardX, guardY;
	State state;
	double playerLastSpotted;
	double moveTimer;
	double timeToMove;
	std::vector<std::pair<int, int>> path;
	size_t pathIndex;

	void MoveAlongPath() {
		if (!path.empty() && pathIndex < path.size() - 1) {
			int nextPathIndex = pathIndex + 1;
			if (isWalkableCallback(path[nextPathIndex].first, path[nextPathIndex].second)) {
				pathIndex++;
				posX = path[pathIndex].first;
				posY = path[pathIndex].second;
			}
		}
	}

	void MoveToGuardPosition() {
		if (!path.empty() && pathIndex > 0) {
			int nextPathIndex = pathIndex - 1;
			if (isWalkableCallback(path[nextPathIndex].first, path[nextPathIndex].second)) {
				pathIndex--;
				posX = path[pathIndex].first;
				posY = path[pathIndex].second;
			}
		}
	}
};
