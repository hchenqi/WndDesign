#pragma once

#include "geometry.h"
#include "margin.h"


BEGIN_NAMESPACE(WndDesign)


constexpr Size ExtendSize(Size size, int length) {
	int width = (int)size.width + length * 2, height = (int)size.height + length * 2;
	return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
}

constexpr Rect ExtendRegion(Rect region, int length) {
	return Rect(region.point - Vector(length, length), ExtendSize(region.size, length));
}

constexpr Size ExtendSize(Size size, Margin margin) {
	int width = (int)size.width + margin.left + margin.right;
	int height = (int)size.height + margin.top + margin.bottom;
	return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
}

constexpr Rect ExtendRegion(Rect region, Margin margin) {
	return Rect(region.point - Vector(margin.left, margin.top), ExtendSize(region.size, margin));
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