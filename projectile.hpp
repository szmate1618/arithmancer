#pragma once

#include <functional>
#include <utility>


class Projectile {
public:
	Projectile(int x, int y, int dx, int dy);

	void Update(double seconds);

	void TryMove();

	std::pair<int, int> GetPosition() const;

private:
	int posX, posY;
	int dx, dy;
	double moveTimer;
	double timeToMove;
};
