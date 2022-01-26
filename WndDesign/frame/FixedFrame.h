#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Auto, class HeightType = Auto>
class FixedFrame;


template<>
class FixedFrame<Auto, Auto> : public WndFrame, public LayoutType<Auto, Auto> {
public:
	FixedFrame(uint height, child_ptr<Auto, Assigned> child) : WndFrame(std::move(child)) {
		size = Size(UpdateChildSizeRef(this->child, size).width, height); SetChildData<uint>(this->child, child_auto_assigned);
	}
	FixedFrame(uint width, child_ptr<Assigned, Auto> child) : WndFrame(std::move(child)) {
		size = Size(width, UpdateChildSizeRef(this->child, size).height); SetChildData<uint>(this->child, child_assigned_auto);
	}
	FixedFrame(Size size, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)) {
		this->size = size; UpdateChildSizeRef(this->child, size); SetChildData<uint>(this->child, child_assigned_assigned);
	}
private:
	static constexpr uint child_auto_assigned = 0;
	static constexpr uint child_assigned_auto = 1;
	static constexpr uint child_assigned_assigned = 2;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		switch (GetChildData<uint>(child)) {
		case child_auto_assigned: if (size.width != child_size.width) { size.width = child_size.width; SizeUpdated(); } break;
		case child_assigned_auto: if (size.height != child_size.height) { size.height = child_size.height; SizeUpdated(); } break;
		}
	}
};


template<>
class FixedFrame<Auto, Assigned> : public WndFrame, public LayoutType<Auto, Assigned> {
public:
	FixedFrame(uint width, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)) { size.width = width; }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size.height != size_ref.height) { size.height = size_ref.height; UpdateChildSizeRef(this->child, size); }
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


template<>
class FixedFrame<Assigned, Auto> : public WndFrame, public LayoutType<Assigned, Auto> {
public:
	FixedFrame(uint height, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)) { size.height = height; }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) { size.width = size_ref.width; UpdateChildSizeRef(this->child, size); }
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


END_NAMESPACE(WndDesign)