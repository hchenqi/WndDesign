#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Point {
	float x;
	float y;

	explicit constexpr Point() : x(), y() {}
	explicit constexpr Point(float x, float y) : x(x), y(y) {}

	constexpr bool operator==(const Point& point) const { return x == point.x && y == point.y; }
	constexpr bool operator!=(const Point& point) const { return x != point.x || y != point.y; }

	constexpr bool operator>(const Point& point) const { return x > point.x && y > point.y; }
	constexpr bool operator<(const Point& point) const { return x < point.x && y < point.y; }
	constexpr bool operator>=(const Point& point) const { return x >= point.x && y >= point.y; }
	constexpr bool operator<=(const Point& point) const { return x <= point.x && y <= point.y; }
};


END_NAMESPACE(WndDesign)