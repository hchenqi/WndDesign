#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Vector {
	int x;
	int y;

	explicit constexpr Vector() :x(0), y(0) {}
	explicit constexpr Vector(int x, int y) : x(x), y(y) {}

	bool operator==(const Vector& vector) const { return x == vector.x && y == vector.y; }
	bool operator!=(const Vector& vector) const { return x != vector.x || y != vector.y; }

	const Vector operator+(const Vector& vector) const { return Vector(x + vector.x, y + vector.y); }
	const Vector operator-(const Vector& vector) const { return Vector(x - vector.x, y - vector.y); }
	Vector& operator+=(const Vector& vector) { return (*this) = (*this) + vector; }
	Vector& operator-=(const Vector& vector) { return (*this) = (*this) - vector; }
};


END_NAMESPACE(WndDesign)