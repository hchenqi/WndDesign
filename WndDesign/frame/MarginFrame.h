#pragma once

#include "WndFrame.h"
#include "../geometry/margin.h"
#include "../style/frame_style.h"


BEGIN_NAMESPACE(WndDesign)


class MarginFrame : public WndFrame {
public:
	using Style = FrameStyle;
public:
	MarginFrame(Style style, child_ptr child);
private:
	Style style;
	Margin margin;
	Size size;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
private:
	Vector GetClientOffset() const { return Vector(margin.left, margin.top); }
	virtual Vector GetChildOffset(WndObject& child) { return GetClientOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)