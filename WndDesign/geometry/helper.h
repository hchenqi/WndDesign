#pragma once

#include "geometry.h"
#include "margin.h"
#include "scale.h"


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


constexpr Vector operator*(Vector vector, Scale scale) {
	return Vector(vector.x * scale.x, vector.y * scale.y);
}

constexpr Point operator*(Point point, Scale scale) {
	return Point(point.x * scale.x, point.y * scale.y);
}

constexpr Size operator*(Size size, Scale scale) {
	return Size(size.width * scale.x, size.height * scale.y);
}

constexpr Rect operator*(Rect rect, Scale scale) {
	return Rect(rect.point * scale, rect.size * scale);
}


inline Rect ceil(Rect rect) {
	float x = floorf(rect.left()), y = floorf(rect.top());
	float width = ceilf(rect.right()) - x, height = ceilf(rect.bottom()) - y;
	return Rect(x, y, width, height);
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