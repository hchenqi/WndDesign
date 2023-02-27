#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class MinFrame : public WndFrame, public LayoutType<Auto, Auto> {
public:
	MinFrame(Size size_min, child_ptr<Relative, Relative> child) : WndFrame(std::move(child)), size_min(size_min) {
		size = UpdateChildSizeRef(this->child, size_min);
		size = Size(std::max(size_min.width, size.width), std::max(size_min.height, size.height));
	}
protected:
	Size size;
	Size size_min;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		child_size = Size(std::max(size_min.width, child_size.width), std::max(size_min.height, child_size.height));
		if (size != child_size) { size = child_size; SizeUpdated(size); }
	}
};


class MaxFrame : public WndFrame, public LayoutType<Auto, Auto> {
public:
	MaxFrame(Size size_max, child_ptr<Relative, Relative> child) : WndFrame(std::move(child)), size_max(size_max) {
		size = UpdateChildSizeRef(this->child, size_max);
		size = Size(std::min(size_max.width, size.width), std::min(size_max.height, size.height));
	}
protected:
	Size size;
	Size size_max;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		child_size = Size(std::min(size_max.width, child_size.width), std::min(size_max.height, child_size.height));
		if (size != child_size) { size = child_size; SizeUpdated(size); }
	}
};


END_NAMESPACE(WndDesign)