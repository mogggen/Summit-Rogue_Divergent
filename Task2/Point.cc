#include "Point.h"
#include "math.h"
#include <iostream>

using namespace std;

point::point()
{

}

point::point(const point &p)
{
	x = p.x;
	y = p.y;
}

point::point(float _x, float _y)
{
	x = _x;
	y = _y;
}

float point::CalcDist(const point &p)
{
	return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}

string point::toString()
{
	return "[" + to_string(x) + ", " + to_string(y) + "]";
}

float point::GetX() { return x; }
float point::GetY() { return y; }

point point::operator+(const point &p)
{
	return point(x + p.x, y + p.y);
}

point point::operator-(const point &p)
{
	return point(x - p.x, y - p.y);
}

void point::operator=(const point &p)
{
	x = p.x;
	y = p.y;
}

bool point::operator==(const point &p)
{
	return x == p.x && y == p.y;
}

bool point::operator!=(const point &p)
{
	return x != p.x || y != p.y;
}