#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class PlaceholderSizeFixed : public WndObject {
public:
	PlaceholderSizeFixed(Size size) : size(size) {}

private:
	Size size;

	void SetSize(Size size) {
		if (this->size != size) {
			this->size = size;
			SizeChanged(size);
		}
	}

	virtual const Size OnSizeRefChange(Size size_ref) override {
		return size;
	}
};


class PlaceHolderWidthAssignedHeightFixed : WndObject {
public:
	PlaceHolderWidthAssignedHeightFixed(uint height) : height(height) {}

private:
	uint height;

	void SetHeight(uint height) {
		if (this->height != height) {
			this->height = height;
			SizeChanged(Size(length_min, height));
		}
	}

	virtual const Size OnSizeRefChange(Size size_ref) override {
		return Size(size_ref.width, height);
	}
};


using PlaceHolderSizeAssigned = WndObject;


END_NAMESPACE(WndDesign)