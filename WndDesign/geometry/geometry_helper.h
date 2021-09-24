#pragma once

#include "geometry.h"
#include "interval.h"
#include "margin.h"


BEGIN_NAMESPACE(WndDesign)


inline uint Clamp(uint length_normal, uint length_min, uint length_max) {
	if (length_normal < length_min) { length_normal = length_min; }
	if (length_normal > length_max) { length_normal = length_max; }
	return length_normal;
}

inline int Clamp(int number, Interval interval) {
	if (number < interval.left()) { return interval.left(); }
	if (number > interval.right()) { return interval.right() - 1; }
	return number;
}

inline Point ClampPointInRegion(Point point, Rect rect) {
	return Point(
		Clamp(point.x, Interval(rect.point.x, rect.size.width)),
		Clamp(point.y, Interval(rect.point.y, rect.size.height))
	);
}

inline uint square(int x) { return (uint)(x * x); };

inline uint SquareDistance(Point a, Point b) { return square(a.x - b.x) + square(a.y - b.y); }

inline float Distance(Point a, Point b) { return (float)sqrt(SquareDistance(a, b)); }

inline float Distance(Point point, Rect rect) { return Distance(point, ClampPointInRegion(point, rect)); }

inline Rect ShrinkRegionBySize(Rect region, Size size) {
	Size new_size = Size(
		(region.size.width > size.width ? region.size.width - size.width : 0) + 1,
		(region.size.height > size.height ? region.size.height - size.height : 0) + 1
	);
	return Rect(region.point, new_size);
}

inline Rect ClampRectInRegion(Rect rect, Rect region) {
	if (rect.size.width > region.size.width) { rect.size.width = region.size.width; }
	if (rect.size.height > region.size.height) { rect.size.height = region.size.height; }
	rect.point = ClampPointInRegion(rect.point, ShrinkRegionBySize(region, rect.size));
	return rect;
}

inline Rect ShrinkRegionByLength(Rect rect, uint length) {
	uint max_length = min(rect.size.width, rect.size.height) / 2;
	if (length > max_length) { length = max_length; }
	Rect region = rect;
	region.point.x += (int)length;
	region.point.y += (int)length;
	region.size.width -= 2 * length;
	region.size.height -= 2 * length;
	return region;
}

inline bool PointInRoundedRectangle(Point point, Rect rect, uint radius) {
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


inline Rect MakeRectFromInterval(Interval horizontal, Interval vertical) {
	return Rect(horizontal.begin, vertical.begin, horizontal.length, vertical.length);
}


inline Point ScalePointBySize(Point point, Size size) {
	return Point(point.x * (int)size.width, point.y * (int)size.height);
}

inline Size ScaleSizeBySize(Size old_size, Size size) {
	return Size(old_size.width * size.width, old_size.height * size.height);
}

inline Rect ScaleRectBySize(Rect region, Size size) {
	return Rect(ScalePointBySize(region.point, size), ScaleSizeBySize(region.size, size));
}


inline int div_floor(int a, int b) { return a >= 0 ? a / b : (a - b + 1) / b; }

inline int div_ceil(int a, int b) { return a <= 0 ? a / b : (a + b - 1) / b; }

inline Rect RegionToOverlappingTileRange(Rect region, Size tile_size) {
	if (tile_size.IsEmpty()) { return region_empty; }
	Point begin_point(div_floor(region.point.x, tile_size.width), div_floor(region.point.y, tile_size.height));
	Point end_point(div_ceil(region.right(), tile_size.width), div_ceil(region.bottom(), tile_size.height));
	Size size = Size(static_cast<uint>(end_point.x - begin_point.x), static_cast<uint>(end_point.y - begin_point.y));
	return Rect(begin_point, size);
}


inline Size ShrinkSizeByMargin(Size size, Margin margin) {
	int width = (int)size.width - (margin.left + margin.right);
	int height = (int)size.height - (margin.top + margin.bottom);
	size.width = width >= 0 ? width : 0;
	size.height = height >= 0 ? height : 0;
	return size;
}

inline Size ExtendSizeByMargin(Size size, Margin margin) {
	return ShrinkSizeByMargin(size, -margin);
}

inline Rect ShrinkRegionByMargin(Rect region, Margin margin) {
	region.point.x += margin.left;
	region.point.y += margin.top;
	region.size = ShrinkSizeByMargin(region.size, margin);
	return region;
}

inline Rect ExtendRegionByMargin(Rect region, Margin margin) {
	return ShrinkRegionByMargin(region, -margin);
}

inline Margin CalculateRelativeMargin(Size size, Rect region) {
	return Margin(
		region.point.x, region.point.y,
		(int)size.width - region.point.x - (int)region.size.width, (int)size.height - region.point.y - (int)region.size.height
	);
}


END_NAMESPACE(WndDesign)