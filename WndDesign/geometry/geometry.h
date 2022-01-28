#pragma once

#include "point.h"
#include "vector.h"
#include "size.h"
#include "rect.h"


BEGIN_NAMESPACE(WndDesign)


constexpr float length_max = 1e8f;
constexpr float length_min = 0.0f;
constexpr Size size_max = Size(length_max, length_max);
constexpr Size size_min = Size(length_min, length_min);
constexpr Size size_empty = size_min;

constexpr float position_max = length_max / 2;
constexpr float position_min = -position_max;
constexpr Point point_max = Point(position_max, position_max);
constexpr Point point_min = Point(position_min, position_min);
constexpr Point point_zero = Point(0.0f, 0.0f);

constexpr Vector vector_zero = Vector(0.0f, 0.0f);

constexpr Rect region_empty = Rect(point_zero, size_empty);
constexpr Rect region_infinite = Rect(point_min, size_max);


constexpr Point operator+(Point begin_point, Vector vector) { return Point(begin_point.x + vector.x, begin_point.y + vector.y); }
constexpr Point operator-(Point end_point, Vector vector) { return Point(end_point.x - vector.x, end_point.y - vector.y); }
constexpr Vector operator-(Point end_point, Point begin_point) { return Vector(end_point.x - begin_point.x, end_point.y - begin_point.y); }

constexpr Rect operator+(Rect rect, Vector vector) { return Rect(rect.point + vector, rect.size); }
constexpr Rect operator-(Rect rect, Vector vector) { return Rect(rect.point - vector, rect.size); }

constexpr Point& operator+=(Point& point, Vector offset) { return point = point + offset; }
constexpr Point& operator-=(Point& point, Vector offset) { return point = point - offset; }
constexpr Rect& operator+=(Rect& rect, Vector offset) { return rect = rect + offset; }
constexpr Rect& operator-=(Rect& rect, Vector offset) { return rect = rect - offset; }


END_NAMESPACE(WndDesign)