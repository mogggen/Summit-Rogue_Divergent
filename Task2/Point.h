#pragma once
#include <string>

class point
{
	float x, y;
public:
	point();
	point(const point &p);
	point(float x, float y);

	float CalcDist(const point &p);
	std::string toString();
	float GetX();
	float GetY();
	point operator+(const point &p);
	point operator-(const point &p);
	void operator=(const point &p);
	bool operator==(const point &p);
	bool operator!=(const point &p);
};