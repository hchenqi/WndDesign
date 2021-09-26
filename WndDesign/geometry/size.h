#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


struct Size {
	uint width;
	uint height;

	explicit constexpr Size() : width(0), height(0) {}
	explicit constexpr Size(uint width, uint height) : width(width), height(height) {}

	constexpr bool operator==(const Size& size) const { return width == size.width && height == size.height; }
	constexpr bool operator!=(const Size& size) const { return width != size.width || height != size.height; }

	constexpr bool IsEmpty() const { return width == 0 || height == 0; }
	constexpr uint Area() const { return width * height; }
};


END_NAMESPACE(WndDesign)