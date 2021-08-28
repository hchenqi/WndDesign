#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class BarLayout;


template<>
class BarLayout<Horizontal> : public WndType<Relative, Relative> {
public:
	using child_ptr = child_ptr<Auto, Relative>;
public:
	BarLayout(child_ptr left, child_ptr right, child_ptr center) :
		left(std::move(left)), right(std::move(right)), center(std::move(center)) {
		RegisterChild(this->left);
		RegisterChild(this->right);
		RegisterChild(this->center);
	}
private:
	child_ptr left;
	child_ptr right;
	child_ptr center;
private:
	Size size;
	uint width_left;
	uint width_right;
	uint width_center;
private:
	const Rect GetRegionLeft() const { return Rect(Point(0, 0), Size(width_left, size.height)); }
	const Rect GetRegionRight() const { return Rect(Point((int)size.width - (int)width_right, 0), Size(width_right, size.height)); }
	const Rect GetRegionCenter() const { return Rect(Point(((int)size.width - (int)width_center) / 2, 0), Size(width_center, size.height)); }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		if (size.height != size_ref.height) {
			size.height = size_ref.height;
			width_left = UpdateChildSizeRef(left, Size(length_min, size.height)).width;
			width_right = UpdateChildSizeRef(right, Size(length_min, size.height)).width;
			width_center = UpdateChildSizeRef(center, Size(length_min, size.height)).width;
		}
		return size;
	}
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {
		if (&child == center.get()) {
			if (width_center != child_size.width) { width_center = child_size.width; }
		} else if (&child == right.get()) {
			if (width_right != child_size.width) { width_right = child_size.width; }
		} else {
			if (width_left != child_size.width) { width_left = child_size.width; }
		}
	}
};


END_NAMESPACE(WndDesign)