#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
private:
	float width, height;
public:
	Rectangle();

	void SetRectangle(Point2D position, float width, float height);
	void SetRotation(float angle, Point2D pivot);
	void render(SDL_Renderer *renderer);
};