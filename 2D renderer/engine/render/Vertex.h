#pragma once
#include "core/MathLibrary.h"

struct Vertex
{
	V3 pos;
	V4 rgba;
	V2 texel;
	V3 normal;

	inline Vertex();
	inline Vertex(V3 pos, V3 rgb);
	inline Vertex(V3 pos, V4 rgba, V2 texel, V3 normal);
};

inline Vertex::Vertex(V3 pos, V3 rgb) : pos(pos), rgba(V4(rgb, 1)), texel(V2()), normal(V3())
{

}

inline Vertex::Vertex(V3 pos, V4 rgba, V2 texel=V2(), V3 normal=V3()) : pos(pos), rgba(rgba), texel(texel), normal(normal)
{

}

