#pragma once

#include "point.h"
#include "size.h"


BEGIN_NAMESPACE(WndDesign)


struct Rect {
	Point point;
	Size size;

	explicit constexpr Rect() : point(), size() {}
	explicit constexpr Rect(Point point, Size size) : point(point), size(size) {}
	explicit constexpr Rect(int x, int y, uint width, uint height) : point(x, y), size(width, height) {}

	constexpr bool operator==(const Rect& rect) const { return point == rect.point && size == rect.size; }
	constexpr bool operator!=(const Rect& rect) const { return point != rect.point || size != rect.size; }

	constexpr int left() const { return point.x; }
	constexpr int right() const { return point.x + (int)size.width; }
	constexpr int top() const { return point.y; }
	constexpr int bottom() const { return point.y + (int)size.height; }

	constexpr Point RightBottom() const { return Point(right(), bottom()); }
	constexpr Point Center() const { return Point(point.x + (int)(size.width / 2), point.y + (int)(size.height / 2)); }

	constexpr bool IsEmpty() const { return size.IsEmpty(); }
	constexpr uint Area() const { return size.Area(); }

	constexpr bool Contains(const Point& point) const { return point >= this->point && point < RightBottom(); }
	constexpr bool Contains(const Rect& rect) const { return rect.point >= point && rect.RightBottom() <= RightBottom(); }

	constexpr Rect Intersect(const Rect& rect) const {
		Point posl1 = point, posl2 = rect.point, posh1 = RightBottom(), posh2 = rect.RightBottom();
		Point posl = Point(max(posl1.x, posl2.x), max(posl1.y, posl2.y));
		Point posh = Point(min(posh1.x, posh2.x), min(posh1.y, posh2.y));
		return posh > posl ? Rect(posl, Size((uint)(posh.x - posl.x), (uint)(posh.y - posl.y))) : Rect();
	}

	constexpr Rect Union(const Rect& rect) const {
		if (IsEmpty()) { return rect; } if (rect.IsEmpty()) { return *this; }
		Point posl1 = point, posl2 = rect.point, posh1 = RightBottom(), posh2 = rect.RightBottom();
		Point posl = Point(min(posl1.x, posl2.x), min(posl1.y, posl2.y));
		Point posh = Point(max(posh1.x, posh2.x), max(posh1.y, posh2.y));
		return Rect(posl, Size(uint(posh.x - posl.x), uint(posh.y - posl.y)));
	}
};


END_NAMESPACE(WndDesign)