#include "Bullet.h"

bool Bullet::GetIsFired()
{
	return isFired;
}

float Bullet::GetBulletAim()
{
	return bulletAim;
}

Point2D Bullet::GetStart()
{
	return start;
}

Point2D Bullet::GetTravel()
{
	return travel;
}

void Bullet::SetIsFired(bool _isFired)
{
	isFired = _isFired;
}

void Bullet::SetBulletAim(float _bulletAim)
{
	bulletAim = _bulletAim;
}

void Bullet::SetStart(Point2D _start)
{
	start = _start;
}

void Bullet::SetTravel(Point2D _travel)
{
	travel = _travel;
}