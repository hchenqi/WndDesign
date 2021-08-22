#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class ScrollFrame;


template<>
class ScrollFrame<Vertical, Relative> : public WndType<Relative, Relative> {
public:
	using child_ptr = child_ptr<Relative, Auto>;
public:
	ScrollFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
private:
	Size size;
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {


		return SetChildSizeRef(child, size_ref);
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) override {
		SizeChanged(child_size);
	}
};


template<>
class ScrollFrame<Vertical, Auto> : public WndType<Relative, Auto> {
public:
	using child_ptr = child_ptr<Relative, Auto>;
public:
	ScrollFrame(uint height_max, child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;


};


END_NAMESPACE(WndDesign)