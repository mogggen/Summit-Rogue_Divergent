#include "Rectangle.h"

Rectangle::Rectangle()
{

}

void Rectangle::SetRectangle(Point2D _pos, float _width, float _height)
{
	pos = _pos;
	width = _width;
	height = _height;
	angle = 0;
	pivot = Point2D(0, 0);
}

void Rectangle::SetRotation(float _angle, Point2D _pivot)
{
	angle = _angle;
	pivot = _pivot;
}

void Rectangle::render(SDL_Renderer *renderer)
{
	int x1, y1, x2, y2;
	for (float i = 0; i < height; i++)
	{
		x1 = (int)round(pivot.GetX() * cos(angle) - (i + pivot.GetY()) * sin(angle));
		y1 = (int)round(pivot.GetX() * sin(angle) + (i + pivot.GetY()) * cos(angle));

		x2 = (int)round(width * cos(angle) - (i + pivot.GetY()) * sin(angle));
		y2 = (int)round(width * sin(angle) + (i + pivot.GetY()) * cos(angle));

		x1 += pos.GetX() + pivot.GetX();
		y1 += pos.GetY() + pivot.GetY();
		x2 += pos.GetX() + pivot.GetX();
		y2 += pos.GetY() + pivot.GetY();

		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	}
}