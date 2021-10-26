#pragma once
#include "shape.h"

class tool
{
public:
	static bool ponitInRect(QPointF point, Shape* shape);
	static bool isShapeIntersect(Shape* rect_1, Shape* rect_2);
};