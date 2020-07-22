#pragma once
#include "Rectangle.h"

class Bullet : public Rectangle
{
	bool isFired;
	float bulletAim;
	Point2D start;
	Point2D travel;
public:
	bool GetIsFired();
	float GetBulletAim();
	Point2D GetStart();
	Point2D GetTravel();

	void SetIsFired(bool isFired);
	void SetBulletAim(float bulletAim);
	void SetStart(Point2D start);
	void SetTravel(Point2D travel);
};