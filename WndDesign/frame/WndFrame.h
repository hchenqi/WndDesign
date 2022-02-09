#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child(std::move(child)) { RegisterChild(this->child); }
	virtual ~WndFrame() override {}
protected:
	child_ptr<> child;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { return SizeUpdated(child_size); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return child; }
	virtual Transform GetChildTransform(WndObject& child) const override { return Transform::Identity(); }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { Redraw(child_redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { DrawChild(child, point_zero, figure_queue, draw_region); }
};


END_NAMESPACE(WndDesign)