#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child((RegisterChild(child), std::move(child))) {}
	virtual ~WndFrame() override {}
protected:
	child_ptr<> child;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child) override { SizeUpdated(); }
	virtual Size GetSize() override { return GetChildSize(child); }
protected:
	virtual void OnChildRedraw(WndObject& child) override { Redraw(); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { DrawChild(child, point_zero, figure_queue, draw_region); }
	virtual Rect GetRedrawRegion() override { return GetChildRedrawRegion(child); }
protected:
	virtual void OnMouseMsg(MouseMsg msg) override { SendChildMouseMsg(child, msg); }
};


END_NAMESPACE(WndDesign)