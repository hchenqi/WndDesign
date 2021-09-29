#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Assigned, class HeightType = Assigned>
class ClipFrame;


template<>
class ClipFrame<Assigned, Assigned> : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	ClipFrame(child_ptr<Assigned, Auto> child) : WndFrame(std::move(child)) {}
	ClipFrame(child_ptr<Auto, Assigned> child) : WndFrame(std::move(child)) {}
	ClipFrame(child_ptr<Auto, Auto> child) : WndFrame(std::move(child)) {}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { UpdateChildSizeRef(child, size_ref); return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


template<>
class ClipFrame<Assigned, Auto> : public WndFrame, public LayoutType<Assigned, Auto> {
public:
	ClipFrame(child_ptr<Auto, Auto> child) : WndFrame(std::move(child)) {
		size.height = UpdateChildSizeRef(this->child, Size()).height;
	}
private:
	Size size;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (size.height != child_size.height) { size.height = child_size.height; SizeUpdated(size); }
	}
};


template<>
class ClipFrame<Auto, Assigned> : public WndFrame, public LayoutType<Auto, Assigned> {
public:
	ClipFrame(child_ptr<Auto, Auto> child) : WndFrame(std::move(child)) {
		size.width = UpdateChildSizeRef(this->child, Size()).width;
	}
private:
	Size size;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { size.height = size_ref.height; return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (size.width != child_size.width) { size.width = child_size.width; SizeUpdated(size); }
	}
};


END_NAMESPACE(WndDesign)