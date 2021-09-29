#pragma once

#include "geometry.h"


BEGIN_NAMESPACE(WndDesign)


constexpr Point ScalePointBySize(Point point, Size size) {
	return Point(point.x * (int)size.width, point.y * (int)size.height);
}

constexpr Size ScaleSizeBySize(Size old_size, Size size) {
	return Size(old_size.width * size.width, old_size.height * size.height);
}

constexpr Rect ScaleRectBySize(Rect region, Size size) {
	return Rect(ScalePointBySize(region.point, size), ScaleSizeBySize(region.size, size));
}


constexpr int div_floor(int a, int b) { return a >= 0 ? a / b : (a - b + 1) / b; }
constexpr int div_ceil(int a, int b) { return a <= 0 ? a / b : (a + b - 1) / b; }

constexpr Rect RegionToOverlappingTileRange(Rect region, Size tile_size) {
	if (tile_size.IsEmpty()) { return region_empty; }
	Point point_begin(div_floor(region.left(), tile_size.width), div_floor(region.top(), tile_size.height));
	Point point_end(div_ceil(region.right(), tile_size.width), div_ceil(region.bottom(), tile_size.height));
	return Rect(point_begin, Size((uint)(point_end.x - point_begin.x), (uint)(point_end.y - point_begin.y)));
}


class RectPointIterator {
private:
	Point start, end, current;
public:
	RectPointIterator(Rect rect) : start(rect.point), end(rect.RightBottom()), current(start) {}
	operator bool() const { return current < end; }
	Point operator*() const { if (!*this) { throw std::logic_error("cannot dereference end iterator"); } return current; }
	void operator++() { current.x++; if (current.x >= end.x) { current.x = start.x; current.y++; } }
};


END_NAMESPACE(WndDesign)