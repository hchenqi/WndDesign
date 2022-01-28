#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Size {
	float width;
	float height;

	explicit constexpr Size() : width(), height() {}
	explicit constexpr Size(float width, float height) : width(width), height(height) {}

	constexpr bool operator==(const Size& size) const { return width == size.width && height == size.height; }
	constexpr bool operator!=(const Size& size) const { return width != size.width || height != size.height; }

	constexpr bool IsEmpty() const { return width == 0.0f || height == 0.0f; }
};


END_NAMESPACE(WndDesign)