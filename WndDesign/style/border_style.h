#pragma once

#include "../figure/color.h"


BEGIN_NAMESPACE(WndDesign)


struct Border {
	float _width;
	float _radius;
	Color _color;

	constexpr Border() : Border(0.0f, 0.0f, color_transparent) {}
	constexpr Border(float width, Color color) : Border(width, 0.0f, color) {}
	constexpr Border(float width, float radius, Color color) : _width(width), _radius(radius), _color(color) {}

	constexpr Border& width(float width) { _width = width; return *this; }
	constexpr Border& radius(float radius) { _radius = radius; return *this; }
	constexpr Border& color(Color color) { _color = color; return *this; }
};


END_NAMESPACE(WndDesign)