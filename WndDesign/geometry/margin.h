#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Margin {
	float left;
	float top;
	float right;
	float bottom;

	explicit constexpr Margin() : Margin(0, 0, 0, 0) {}
	explicit constexpr Margin(float all) : Margin(all, all, all, all) {}
	explicit constexpr Margin(float left_right, float top_bottom) : Margin(left_right, top_bottom, left_right, top_bottom) {}
	explicit constexpr Margin(float left_right, float top, float bottom) : Margin(left_right, top, left_right, bottom) {}
	explicit constexpr Margin(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}

	constexpr Margin operator-() const { return Margin(-left, -top, -right, -bottom); }

	constexpr Margin operator+(const Margin& margin) const {
		return Margin(left + margin.left, top + margin.top, right + margin.right, bottom + margin.bottom);
	}
};


END_NAMESPACE(WndDesign)