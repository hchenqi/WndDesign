#pragma once

#include "point.h"
#include "size.h"


BEGIN_NAMESPACE(WndDesign)


struct Rect {
	Point point;
	Size size;

	explicit constexpr Rect() : point(), size() {}
	explicit constexpr Rect(Point point, Size size) : point(point), size(size) {}
	explicit constexpr Rect(float x, float y, float width, float height) : point(x, y), size(width, height) {}

	constexpr bool operator==(const Rect& rect) const { return point == rect.point && size == rect.size; }
	constexpr bool operator!=(const Rect& rect) const { return point != rect.point || size != rect.size; }

	constexpr float left() const { return point.x; }
	constexpr float right() const { return point.x + size.width; }
	constexpr float top() const { return point.y; }
	constexpr float bottom() const { return point.y + size.height; }

	constexpr Point RightBottom() const { return Point(right(), bottom()); }
	constexpr Point Center() const { return Point(point.x + size.width / 2, point.y + size.height / 2); }

	constexpr bool IsEmpty() const { return size.IsEmpty(); }

	constexpr bool Contains(const Point& point) const { return point >= this->point && point < RightBottom(); }
	constexpr bool Contains(const Rect& rect) const { return rect.point >= point && rect.RightBottom() <= RightBottom(); }

	constexpr Rect Intersect(const Rect& rect) const {
		Point posl1 = point, posl2 = rect.point, posh1 = RightBottom(), posh2 = rect.RightBottom();
		Point posl = Point(max(posl1.x, posl2.x), max(posl1.y, posl2.y));
		Point posh = Point(min(posh1.x, posh2.x), min(posh1.y, posh2.y));
		return posh > posl ? Rect(posl, Size(posh.x - posl.x, posh.y - posl.y)) : Rect();
	}

	constexpr Rect Union(const Rect& rect) const {
		if (IsEmpty()) { return rect; } if (rect.IsEmpty()) { return *this; }
		Point posl1 = point, posl2 = rect.point, posh1 = RightBottom(), posh2 = rect.RightBottom();
		Point posl = Point(min(posl1.x, posl2.x), min(posl1.y, posl2.y));
		Point posh = Point(max(posh1.x, posh2.x), max(posh1.y, posh2.y));
		return Rect(posl, Size(posh.x - posl.x, posh.y - posl.y));
	}
};


END_NAMESPACE(WndDesign)