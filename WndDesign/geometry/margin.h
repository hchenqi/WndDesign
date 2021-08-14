#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Margin {
	int left;
	int top;
	int right;
	int bottom;

	explicit constexpr Margin() :left(), top(), right(), bottom() {}
	explicit constexpr Margin(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) {}

	const Margin operator+(const Margin& margin) const {
		return Margin(left + margin.left, top + margin.top, right + margin.right, bottom + margin.bottom);
	}
};


END_NAMESPACE(WndDesign)