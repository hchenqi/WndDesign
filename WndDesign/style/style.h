#pragma once

#include "value_tag.h"
#include "../figure/color.h"


BEGIN_NAMESPACE(WndDesign)


struct LengthStyle {
public:
	ValueTag _normal = length_auto;
	ValueTag _min = length_min_tag;
	ValueTag _max = length_max_tag;
	constexpr LengthStyle& normal(ValueTag normal) { _normal = normal; return *this; }
	constexpr LengthStyle& min(ValueTag min) { _min = min; return *this; }
	constexpr LengthStyle& max(ValueTag max) { _max = max; return *this; }
	constexpr void setFixed(ValueTag length) { _normal = _min = _max = length; }
};


struct PositionStyle {
public:
	ValueTag _left = position_auto;
	ValueTag _top = position_auto;
	ValueTag _right = position_auto;
	ValueTag _bottom = position_auto;
public:
	constexpr PositionStyle& left(ValueTag left) { _left = left; return *this; }
	constexpr PositionStyle& top(ValueTag top) { _top = top; return *this; }
	constexpr PositionStyle& right(ValueTag right) { _right = right; return *this; }
	constexpr PositionStyle& bottom(ValueTag bottom) { _bottom = bottom; return *this; }
	constexpr void set(ValueTag left, ValueTag top, ValueTag right, ValueTag bottom) { _left = left; _top = top; _right = right; _bottom = bottom; }
	constexpr void setAll(ValueTag all) { _left = _top = _right = _bottom = all; }
	constexpr PositionStyle& setHorizontalCenter() { _left = _right = position_center; return *this; }
	constexpr PositionStyle& setVerticalCenter() { _top = _bottom = position_center; return *this; }
};


END_NAMESPACE(WndDesign)