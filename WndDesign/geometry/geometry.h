#pragma once

#include "point.h"
#include "vector.h"
#include "size.h"
#include "rect.h"


BEGIN_NAMESPACE(WndDesign)


constexpr uint length_max = 0x0FFFFFFF;
constexpr uint length_min = 0;
constexpr Size size_max = Size(length_max, length_max);
constexpr Size size_min = Size(length_min, length_min);
constexpr Size size_empty = size_min;

constexpr int position_max = (int)length_max / 2;
constexpr int position_min = -position_max;
constexpr Point point_max = Point(position_max, position_max);
constexpr Point point_min = Point(position_min, position_min);
constexpr Point point_zero = Point(0, 0);

constexpr Vector vector_zero = Vector(0, 0);

constexpr Rect region_empty = Rect(point_zero, size_empty);
constexpr Rect region_infinite = Rect(point_min, size_max);


// begin_point + vector = end_point
inline const Point operator+(const Point& begin_point, const Vector& vector) {
	return Point(begin_point.x + vector.x, begin_point.y + vector.y);
}

// end_point - vector = begin_point
inline const Point operator-(const Point& end_point, const Vector& vector) {
	return Point(end_point.x - vector.x, end_point.y - vector.y);
}

// end_point - begin_point = vector
inline const Vector operator-(const Point& end_point, const Point& begin_point) {
	return Vector(end_point.x - begin_point.x, end_point.y - begin_point.y);
}


inline const Rect operator+(const Rect& rect, const Vector& vector) {
	return Rect(rect.point + vector, rect.size);
}

inline const Rect operator-(const Rect& rect, const Vector& vector) {
	return Rect(rect.point - vector, rect.size);
}


inline Point& operator+=(Point& point, Vector offset) { return point = point + offset;}
inline Point& operator-=(Point& point, Vector offset) {return point = point - offset;}
inline Rect& operator+=(Rect& rect, Vector offset) {return rect = rect + offset;}
inline Rect& operator-=(Rect& rect, Vector offset) {return rect = rect - offset;}


END_NAMESPACE(WndDesign)