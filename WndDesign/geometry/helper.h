#pragma once

#include "geometry.h"
#include "margin.h"


BEGIN_NAMESPACE(WndDesign)


constexpr Size Extend(Size size, float length) {
	float width = size.width + length * 2, height = size.height + length * 2;
	return Size(width >= 0.0f ? width : 0.0f, height >= 0.0f ? height : 0.0f);
}

constexpr Rect Extend(Rect region, float length) {
	return Rect(region.point - Vector(length, length), Extend(region.size, length));
}

constexpr Size Extend(Size size, Margin margin) {
	float width = size.width + margin.left + margin.right;
	float height = size.height + margin.top + margin.bottom;
	return Size(width >= 0.0f ? width : 0.0f, height >= 0.0f ? height : 0.0f);
}

constexpr Rect Extend(Rect region, Margin margin) {
	return Rect(region.point - Vector(margin.left, margin.top), Extend(region.size, margin));
}


constexpr Vector Scale(Vector vector, float scale_x, float scale_y) {
	return Vector(vector.x * scale_x, vector.y * scale_y);
}

constexpr Point Scale(Point point, float scale_x, float scale_y) {
	return Point(point.x * scale_x, point.y * scale_y);
}

constexpr Size Scale(Size size, float scale_x, float scale_y) {
	return Size(size.width * scale_x, size.height * scale_y);
}

constexpr Rect Scale(Rect rect, float scale_x, float scale_y) {
	return Rect(Scale(rect.point, scale_x, scale_y), Scale(rect.size, scale_x, scale_y));
}


constexpr float square(float x) { return x * x; };

constexpr float square_distance(Point a, Point b) { return square(a.x - b.x) + square(a.y - b.y); }

constexpr bool PointInRoundedRectangle(Point point, Rect rect, float radius) {
	if (!rect.Contains(point)) { return false; } if (radius == 0.0f) { return true; }
	float x1 = rect.left() + radius, x2 = rect.right() - radius;
	float y1 = rect.top() + radius, y2 = rect.bottom() - radius;
	float x = point.x, y = point.y;
	if (x < x1 && y < y1) { return square(x - x1) + square(y - y1) <= square(radius); }
	if (x < x1 && y > y2) { return square(x - x1) + square(y - y2) <= square(radius); }
	if (x > x2 && y < y1) { return square(x - x2) + square(y - y1) <= square(radius); }
	if (x > x2 && y > y2) { return square(x - x2) + square(y - y2) <= square(radius); }
	return true;
}


END_NAMESPACE(WndDesign)