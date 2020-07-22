#pragma once
#include <string>

class Point2D
{
	float x, y;
public:
	Point2D();
	Point2D(const Point2D &p);
	Point2D(float x, float y);

	float CalcDist(const Point2D &p);
	std::string toString();
	float GetX();
	float GetY();
	Point2D operator+(const Point2D &p);
	Point2D operator-(const Point2D &p);
	void operator=(const Point2D &p);
	bool operator==(const Point2D &p);
	bool operator!=(const Point2D &p);
};