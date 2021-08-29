#pragma once

#include "../figure/color.h"


BEGIN_NAMESPACE(WndDesign)


struct FrameStyle {
	struct BorderStyle {
	public:
		ushort _width;
		ushort _radius;
		Color _color;
	public:
		constexpr BorderStyle& width(ushort width) { _width = width; return *this; }
		constexpr BorderStyle& radius(ushort radius) { _radius = radius; return *this; }
		constexpr BorderStyle& color(Color color) { _color = color; return *this; }
	}border;

	struct PaddingStyle {
	public:
		uchar _left;
		uchar _top;
		uchar _right;
		uchar _bottom;
	public:
		constexpr PaddingStyle& left(uchar left) { _left = left; return *this; }
		constexpr PaddingStyle& top(uchar top) { _top = top; return *this; }
		constexpr PaddingStyle& right(uchar right) { _right = right; return *this; }
		constexpr PaddingStyle& bottom(uchar bottom) { _bottom = bottom; return *this; }
		constexpr void set(uchar left, uchar top, uchar right, uchar bottom) { _left = left; _top = top; _right = right; _bottom = bottom; }
		constexpr void set(uchar left_right, uchar top, uchar bottom) { _left = _right = left_right; _top = top; _bottom = bottom; }
		constexpr void set(uchar left_right, uchar top_bottom) { _left = _right = left_right; _top = _bottom = top_bottom; }
		constexpr void set(uchar value) { _left = _top = _right = _bottom = value; }
	}padding;

	struct BackgroundStyle {
	public:
		Color _color;
	public:
		constexpr BackgroundStyle& color(Color color) { _color = color; return *this; }
	}background;
};


END_NAMESPACE(WndDesign)