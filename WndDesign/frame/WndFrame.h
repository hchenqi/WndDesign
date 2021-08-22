#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class WndFrame : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	WndFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		return SetChildSizeRef(child, size_ref);
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) override {
		SizeChanged(child_size);
	}
};


template<class WidthType, class HeightType>
class WndFrameMutable : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	WndFrameMutable(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
public:
	void SetChild(child_ptr child) {
		this->child = std::move(child);
		SizeChanged(SetChildSizeRef(child, size_ref));
	}
private:
	Size size_ref;
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		this->size_ref = size_ref;
		return SetChildSizeRef(child, size_ref);
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) override {
		SizeChanged(child_size);
	}
};


END_NAMESPACE(WndDesign)