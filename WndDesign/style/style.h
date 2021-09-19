#pragma once

#include "value_tag.h"
#include "../figure/color.h"


BEGIN_NAMESPACE(WndDesign)


struct Style {
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

	struct BorderStyle {
	public:
		ushort _width = 0;
		ushort _radius = 0;
		Color _color = color_transparent;
	public:
		constexpr BorderStyle& width(ushort width) { _width = width; return *this; }
		constexpr BorderStyle& radius(ushort radius) { _radius = radius; return *this; }
		constexpr BorderStyle& color(Color color) { _color = color; return *this; }
	};

	struct PaddingStyle {
	public:
		uchar _left = 0;
		uchar _top = 0;
		uchar _right = 0;
		uchar _bottom = 0;
	public:
		constexpr PaddingStyle& left(uchar left) { _left = left; return *this; }
		constexpr PaddingStyle& top(uchar top) { _top = top; return *this; }
		constexpr PaddingStyle& right(uchar right) { _right = right; return *this; }
		constexpr PaddingStyle& bottom(uchar bottom) { _bottom = bottom; return *this; }
		constexpr void set(uchar left, uchar top, uchar right, uchar bottom) { _left = left; _top = top; _right = right; _bottom = bottom; }
		constexpr void set(uchar left_right, uchar top, uchar bottom) { _left = _right = left_right; _top = top; _bottom = bottom; }
		constexpr void set(uchar left_right, uchar top_bottom) { _left = _right = left_right; _top = _bottom = top_bottom; }
		constexpr void set(uchar value) { _left = _top = _right = _bottom = value; }
	};
};


END_NAMESPACE(WndDesign)