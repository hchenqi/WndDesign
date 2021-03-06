#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Assigned, class HeightType = Assigned>
class CenterFrame;


class _CenterFrame_Base : public WndFrame {
protected:
	_CenterFrame_Base(child_ptr<> child) : WndFrame(std::move(child)) {}
protected:
	Size size;
	Size child_size;
protected:
	Vector GetChildOffset() const {
		return Vector((size.width - child_size.width) / 2, (size.height - child_size.height) / 2);
	}
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return GetChildOffset(); }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Redraw(child_redraw_region + GetChildOffset());
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		return DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
};


template<>
class CenterFrame<Assigned, Assigned> : public _CenterFrame_Base, public LayoutType<Assigned, Assigned> {
public:
	CenterFrame(child_ptr<Assigned, Auto> child) : _CenterFrame_Base(std::move(child)) {}
	CenterFrame(child_ptr<Auto, Assigned> child) : _CenterFrame_Base(std::move(child)) {}
	CenterFrame(child_ptr<Auto, Auto> child) : _CenterFrame_Base(std::move(child)) {}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) { size = size_ref; child_size = UpdateChildSizeRef(child, size); } return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { this->child_size = child_size; }
};


template<>
class CenterFrame<Assigned, Auto> : public _CenterFrame_Base, public LayoutType<Assigned, Auto> {
public:
	CenterFrame(child_ptr<Auto, Auto> child) : _CenterFrame_Base(std::move(child)) {
		child_size = UpdateChildSizeRef(this->child, Size());
		size.height = child_size.height;
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		this->child_size = child_size;
		if (size.height != child_size.height) { size.height = child_size.height; SizeUpdated(size); }
	}
};


template<>
class CenterFrame<Auto, Assigned> : public _CenterFrame_Base, public LayoutType<Auto, Assigned> {
public:
	CenterFrame(child_ptr<Auto, Auto> child) : _CenterFrame_Base(std::move(child)) {
		child_size = UpdateChildSizeRef(this->child, Size());
		size.width = child_size.width;
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { size.height = size_ref.height; return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		this->child_size = child_size;
		if (size.width != child_size.width) { size.width = child_size.width; SizeUpdated(size); }
	}
};


END_NAMESPACE(WndDesign)