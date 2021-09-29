#pragma once

#include "geometry.h"
#include "interval.h"


BEGIN_NAMESPACE(WndDesign)


constexpr int clamp(int number, Interval range) {
	if (number < range.left()) { return range.left(); }
	if (number >= range.right()) { return range.right() - 1; }
	return number;
}

constexpr int clamp(Interval interval, Interval range) {
	return clamp(clamp(interval.right(), range) - (int)interval.length, range);
}

constexpr Point clamp(Point point, Rect region) {
	return Point(clamp(point.x, Interval(region.point.x, region.size.width)), clamp(point.y, Interval(region.point.y, region.size.height)));
}

constexpr Point clamp(Rect rect, Rect region) {
	return clamp(clamp(rect.RightBottom(), region) - Vector((int)rect.size.width, (int)rect.size.height), region);
}


END_NAMESPACE(WndDesign)