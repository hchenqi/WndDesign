#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Assigned, class HeightType = Assigned>
class ClipFrame;


template<>
class ClipFrame<Assigned, Assigned> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	ClipFrame(child_ptr<> child) : WndFrame(std::move(child)) {}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { UpdateChildSizeRef(child, size_ref); return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { Redraw(region_infinite); }
};


template<>
class ClipFrame<Assigned, Auto> : public WndFrame, public LayoutType<Assigned, Auto> {
public:
	ClipFrame(child_ptr<Relative, Auto> child) : WndFrame(std::move(child)) {}
protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size.width = size_ref.width;
		size.height = UpdateChildSizeRef(child, size).height;
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (size.height != child_size.height) {
			size.height = child_size.height;
			SizeUpdated(size);
		} else {
			Redraw(region_infinite);
		}
	}
};


template<>
class ClipFrame<Auto, Assigned> : public WndFrame, public LayoutType<Auto, Assigned> {
public:
	ClipFrame(child_ptr<Auto, Relative> child) : WndFrame(std::move(child)) {}
protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		size.height = size_ref.height;
		size.width = UpdateChildSizeRef(child, size).width;
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (size.width != child_size.width) {
			size.width = child_size.width;
			SizeUpdated(size);
		} else {
			Redraw(region_infinite);
		}
	}
};


END_NAMESPACE(WndDesign)