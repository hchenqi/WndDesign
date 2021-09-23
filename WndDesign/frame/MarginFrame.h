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
	virtual const Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
private:
	const Vector GetClientOffset() const { return Vector(margin.left, margin.top); }
	virtual const Vector GetChildOffset(WndObject& child) const { return GetClientOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override;
};


END_NAMESPACE(WndDesign)