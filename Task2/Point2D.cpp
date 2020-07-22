#include "Point2D.h"
#include "math.h"
#include <iostream>

using namespace std;

Point2D::Point2D()
{

}

Point2D::Point2D(const Point2D &p)
{
	x = p.x;
	y = p.y;
}

Point2D::Point2D(float _x, float _y)
{
	x = _x;
	y = _y;
}

float Point2D::CalcDist(const Point2D &p)
{
	return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}

string Point2D::toString()
{
	return "[" + to_string(x) + ", " + to_string(y) + "]";
}

float Point2D::GetX() { return x; }
float Point2D::GetY() { return y; }

Point2D Point2D::operator+(const Point2D &p)
{
	return Point2D(x + p.x, y + p.y);
}

Point2D Point2D::operator-(const Point2D &p)
{
	return Point2D(x - p.x, y - p.y);
}

void Point2D::operator=(const Point2D &p)
{
	x = p.x;
	y = p.y;
}

bool Point2D::operator==(const Point2D &p)
{
	return x == p.x && y == p.y;
}

bool Point2D::operator!=(const Point2D &p)
{
	return x != p.x || y != p.y;
}