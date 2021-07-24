#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Point {
	int x;
	int y;

	explicit constexpr Point() : x(0), y(0) {}
	explicit constexpr Point(int x, int y) : x(x), y(y) {}

	bool operator==(const Point& point) const { return x == point.x && y == point.y; }
	bool operator!=(const Point& point) const { return x != point.x || y != point.y; }

	bool operator>(const Point& point) const { return x > point.x && y > point.y; }
	bool operator<(const Point& point) const { return x < point.x&& y < point.y; }
	bool operator>=(const Point& point) const { return x >= point.x && y >= point.y; }
	bool operator<=(const Point& point) const { return x <= point.x && y <= point.y; }
};


END_NAMESPACE(WndDesign)