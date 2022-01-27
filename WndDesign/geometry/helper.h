#pragma once

#include "geometry.h"
#include "margin.h"


BEGIN_NAMESPACE(WndDesign)


constexpr Size Extend(Size size, int length) {
	int width = (int)size.width + length * 2, height = (int)size.height + length * 2;
	return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
}

constexpr Rect Extend(Rect region, int length) {
	return Rect(region.point - Vector(length, length), Extend(region.size, length));
}

constexpr Size Extend(Size size, Margin margin) {
	int width = (int)size.width + margin.left + margin.right;
	int height = (int)size.height + margin.top + margin.bottom;
	return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
}

constexpr Rect Extend(Rect region, Margin margin) {
	return Rect(region.point - Vector(margin.left, margin.top), Extend(region.size, margin));
}


constexpr Vector Scale(Vector vector, float scale_x, float scale_y) {
	return Vector((int)roundf(vector.x * scale_x), (int)roundf(vector.y * scale_y));
}

constexpr Point Scale(Point point, float scale_x, float scale_y) {
	return Point((int)roundf(point.x * scale_x), (int)roundf(point.y * scale_y));
}

constexpr Size Scale(Size size, float scale_x, float scale_y) {
	return Size((uint)roundf(size.width * scale_x), (uint)roundf(size.height * scale_y));
}

constexpr Rect Scale(Rect rect, float scale_x, float scale_y) {
	return Rect(Scale(rect.point, scale_x, scale_y), Scale(rect.size, scale_x, scale_y));
}


constexpr uint square(int x) { return (uint)(x * x); };

constexpr bool PointInRoundedRectangle(Point point, Rect rect, uint radius) {
	if (!rect.Contains(point)) { return false; } if (radius == 0) { return true; }
	int x1 = rect.left() + (int)radius, x2 = rect.right() - (int)radius;
	int	y1 = rect.top() + (int)radius, y2 = rect.bottom() - (int)radius;
	int x = point.x, y = point.y;
	if (x < x1 && y < y1) { return square(x - x1) + square(y - y1) <= square(radius); }
	if (x < x1 && y > y2) { return square(x - x1) + square(y - y2) <= square(radius); }
	if (x > x2 && y < y1) { return square(x - x2) + square(y - y1) <= square(radius); }
	if (x > x2 && y > y2) { return square(x - x2) + square(y - y2) <= square(radius); }
	return true;
}


END_NAMESPACE(WndDesign)