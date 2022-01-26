#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child((RegisterChild(child), std::move(child))) {}
	virtual ~WndFrame() override {}
protected:
	child_ptr<> child;
	Size size;
	Rect redraw_region;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size = UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { if (size != child_size) { size = child_size; SizeUpdated(); } }
	virtual Size GetSize() override { return size; }
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return vector_zero; }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return child; }
protected:
	virtual Rect GetRedrawRegion() override { return redraw_region; }
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { redraw_region = child_redraw_region; Redraw(); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { DrawChild(child, point_zero, figure_queue, draw_region); }
};


END_NAMESPACE(WndDesign)