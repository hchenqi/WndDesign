#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Vector {
	int x;
	int y;

	explicit constexpr Vector() :x(0), y(0) {}
	explicit constexpr Vector(int x, int y) : x(x), y(y) {}

	constexpr bool operator==(const Vector& vector) const { return x == vector.x && y == vector.y; }
	constexpr bool operator!=(const Vector& vector) const { return x != vector.x || y != vector.y; }

	constexpr Vector operator-() const { return Vector(-x, -y); }

	constexpr Vector operator+(const Vector& vector) const { return Vector(x + vector.x, y + vector.y); }
	constexpr Vector operator-(const Vector& vector) const { return Vector(x - vector.x, y - vector.y); }
	constexpr Vector& operator+=(const Vector& vector) { return (*this) = (*this) + vector; }
	constexpr Vector& operator-=(const Vector& vector) { return (*this) = (*this) - vector; }
};


END_NAMESPACE(WndDesign)