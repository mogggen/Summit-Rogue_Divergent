#pragma once
#include "SDL.h"
#include "Point.h"
class Circle
{
	point pos;
	float radius;
public:
	void SetCircle(point position, float radius);
	void render(SDL_Renderer *renderer);
};