#include "Circle.h"

Circle::Circle()
{

}

void Circle::SetCircle(Point2D _pos, float _radius)
{
	pos = _pos;
	radius = _radius;
}

void Circle::render(SDL_Renderer *renderer)
{
	for (int h = 0; h < radius * 2; h++)
	{
		int x1, x2;
		int dy = radius - h;
		int dx = SDL_sqrt(radius * radius - dy * dy);
		x1 = dx;
		x2 = -dx;
		SDL_RenderDrawLine(renderer, pos.GetX() + radius + x1, pos.GetY() + radius + dy, pos.GetX() + radius + x2, pos.GetY() + radius + dy);
	}
}