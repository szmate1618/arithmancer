#include "projectile.hpp"


Projectile::Projectile(int x, int y, int dx, int dy) :
	posX(x), posY(y),
	dx(dx), dy(dy),
	moveTimer(0.0), timeToMove(0.1)
{
}

void Projectile::Update(double seconds) {
	moveTimer += seconds;
	TryMove();
}

void Projectile::TryMove()
{
	if (moveTimer >= timeToMove) {
		moveTimer = 0.0;

		posX += dx;
		posY += dy;
	}
}

std::pair<int, int> Projectile::GetPosition() const {
	return { posX, posY };
}
