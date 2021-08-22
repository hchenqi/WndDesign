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
	int GetRightPosition() const { return (int)size.width - (int)width_right; }
	int GetCenterPosition() const { return ((int)size.width - (int)width_center) / 2; }
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (size.height != size_ref.height) {
			size.height = size_ref.height;
			width_left = SetChildSizeRef(left, Size(length_min, size.height)).width;
			width_right = SetChildSizeRef(right, Size(length_min, size.height)).width;
			width_center = SetChildSizeRef(center, Size(length_min, size.height)).width;
		}
		return size;
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
		if (&child == center.get()) {
			if (width_center != child_size.width) {
				width_center = child_size.width;
			}
		} else if (&child == right.get()) {
			if (width_right != child_size.width) {
				width_right = child_size.width;
			}
		} else {
			if (width_left != child_size.width) {
				width_left = child_size.width;
			}
		}
	}
};


END_NAMESPACE(WndDesign)