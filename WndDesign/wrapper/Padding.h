#pragma once

#include "wrapper.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd>
class Padding : public Wnd {
public:
	using Wnd::Wnd;

	// style
protected:
	using Style = Margin;
	Style padding;
private:
	Vector GetInnerOffset() const { return Vector(padding.left, padding.top); }

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { Wnd::OnSizeRefUpdate(Extend(size_ref, -padding)); }
	virtual Size GetSize() override { return Extend(Wnd::GetSize(), padding); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		Point child_point = point - GetInnerOffset(); ref_ptr<WndObject> child = Wnd::HitTest(child_point);
		return child == this ? this : (point = child_point, child);
	}
	virtual Transform GetChildTransform(WndObject& child) override { return Wnd::GetChildTransform(child) * GetInnerOffset(); }

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Offset(GetInnerOffset(), [&]() { Wnd::OnDraw(figure_queue, draw_region - GetInnerOffset()); });
	}
	virtual Rect GetRedrawRegion() override { return Wnd::GetRedrawRegion() + GetInnerOffset(); }

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override { msg.point -= GetInnerOffset(); Wnd::OnMouseMsg(msg); }
};


END_NAMESPACE(WndDesign)