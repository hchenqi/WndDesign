#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Auto, class HeightType = Auto>
class FixedFrame;


template<>
class FixedFrame<Auto, Auto> : public WndFrame, public LayoutType<Auto, Auto> {
public:
	FixedFrame(float height, child_ptr<Auto, Assigned> child) : WndFrame(std::move(child)) {
		size = Size(UpdateChildSizeRef(this->child, size).width, height); SetChildData<uint>(this->child, child_auto_assigned);
	}
	FixedFrame(float width, child_ptr<Assigned, Auto> child) : WndFrame(std::move(child)) {
		size = Size(width, UpdateChildSizeRef(this->child, size).height); SetChildData<uint>(this->child, child_assigned_auto);
	}
	FixedFrame(Size size, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)) {
		this->size = size; UpdateChildSizeRef(this->child, size); SetChildData<uint>(this->child, child_assigned_assigned);
	}
protected:
	Size size;
protected:
	static constexpr uint child_auto_assigned = 0;
	static constexpr uint child_assigned_auto = 1;
	static constexpr uint child_assigned_assigned = 2;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		switch (GetChildData<uint>(child)) {
		case child_auto_assigned: if (size.width != child_size.width) { size.width = child_size.width; SizeUpdated(size); } break;
		case child_assigned_auto: if (size.height != child_size.height) { size.height = child_size.height; SizeUpdated(size); } break;
		}
	}
};


template<>
class FixedFrame<Auto, Assigned> : public WndFrame, public LayoutType<Auto, Assigned> {
public:
	FixedFrame(float width, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)), size(width, 0) {}
protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.height != size_ref.height) { size.height = size_ref.height; UpdateChildSizeRef(this->child, size); } return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


template<>
class FixedFrame<Assigned, Auto> : public WndFrame, public LayoutType<Assigned, Auto> {
public:
	FixedFrame(float height, child_ptr<Assigned, Assigned> child) : WndFrame(std::move(child)), size(0, height) {}
protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) { size.width = size_ref.width; UpdateChildSizeRef(this->child, size); } return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


END_NAMESPACE(WndDesign)