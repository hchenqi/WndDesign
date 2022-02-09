#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class MaxFrame : public WndFrame, public LayoutType<Auto, Auto> {
public:
	MaxFrame(float width_max, child_ptr<Relative, Auto> child) : WndFrame(std::move(child)), width_max(width_max) {
		size = UpdateChildSizeRef(this->child, Size(width_max, 0)); size.width = min(width_max, size.width);
	}
protected:
	float width_max;
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		child_size.width = min(width_max, child_size.width);
		if (size != child_size) { size = child_size; SizeUpdated(size); }
	}
};


END_NAMESPACE(WndDesign)