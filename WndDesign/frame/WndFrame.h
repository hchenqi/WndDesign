#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child(std::move(child)) { RegisterChild(this->child); }
	virtual ~WndFrame() override {}
protected:
	child_ptr<> child;
	Size size = size_empty;
	Rect redraw_region = region_infinite;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size = UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { if (size != child_size) { size = child_size; SizeUpdated(); } }
	virtual Size GetSize() override { return size; }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return child; }
	virtual Transform GetChildTransform(WndObject& child) override { return Transform::Identity(); }
protected:
	virtual Rect GetRedrawRegion() override { return redraw_region; }
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { redraw_region = child_redraw_region; Redraw(); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { DrawChild(child, point_zero, figure_queue, draw_region); }
};


END_NAMESPACE(WndDesign)