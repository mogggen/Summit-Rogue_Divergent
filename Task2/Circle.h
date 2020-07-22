#pragma once
#include "Shape.h"
class Circle : public Shape
{
	float radius;
public:
	Circle();
	void SetCircle(Point2D position, float radius);
	void render(SDL_Renderer *renderer);
};