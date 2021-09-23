#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ClipFrame;


template<>
class ClipFrame<Vertical> : public WndFrame, public LayoutType<Relative, Relative> {
public:
	using child_ptr = WndDesign::child_ptr<Relative, Auto>;
public:
	ClipFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	uint child_height = 0;
	const Rect GetClipRegion() const { return Rect(point_zero, Size(length_max, child_height)); }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		child_height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height; return Size();
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { child_height = child_size.height; }
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.y < child_height) { return child; }
		return nullptr;
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		Redraw(GetClipRegion().Intersect(redraw_region));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {
		DrawChild(child, point_zero, figure_queue, GetClipRegion().Intersect(draw_region));
	}
};


END_NAMESPACE(WndDesign)