#pragma once

#include "geometry.h"
#include "interval.h"
#include "margin.h"


BEGIN_NAMESPACE(WndDesign)


constexpr uint Clamp(uint length_normal, uint length_min, uint length_max) {
	if (length_normal < length_min) { length_normal = length_min; }
	if (length_normal > length_max) { length_normal = length_max; }
	return length_normal;
}

constexpr int Clamp(int number, Interval interval) {
	if (number < interval.left()) { return interval.left(); }
	if (number > interval.right()) { return interval.right() - 1; }
	return number;
}

constexpr Point ClampPointInRegion(Point point, Rect rect) {
	return Point(
		Clamp(point.x, Interval(rect.point.x, rect.size.width)),
		Clamp(point.y, Interval(rect.point.y, rect.size.height))
	);
}

constexpr Rect ShrinkRegionBySize(Rect region, Size size) {
	Size new_size = Size(
		(region.size.width > size.width ? region.size.width - size.width : 0) + 1,
		(region.size.height > size.height ? region.size.height - size.height : 0) + 1
	);
	return Rect(region.point, new_size);
}

constexpr Rect ClampRectInRegion(Rect rect, Rect region) {
	if (rect.size.width > region.size.width) { rect.size.width = region.size.width; }
	if (rect.size.height > region.size.height) { rect.size.height = region.size.height; }
	rect.point = ClampPointInRegion(rect.point, ShrinkRegionBySize(region, rect.size));
	return rect;
}


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


END_NAMESPACE(WndDesign)