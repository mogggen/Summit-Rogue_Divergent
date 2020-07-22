#include "Shape.h"

Shape::Shape()
{

}

Shape::~Shape()
{

}

Shape::Shape(Point2D _pos)
{
	pos = _pos;
	angle = 0;
	pivot = Point2D(0, 0);
}

Shape::Shape(Point2D _pos, float _angle, Point2D _pivot)
{
	pos = _pos;
	angle = _angle;
	pivot = _pivot;
}

void Shape::render(SDL_Renderer *renderer)
{
	
};