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
	virtual void OnSizeRefUpdate(Size size_ref) override { Wnd::OnSizeRefUpdate(ExtendSize(size_ref, -padding)); }
	virtual Size GetSize() override { return ExtendSize(Wnd::GetSize(), padding); }
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return Wnd::GetChildOffset(child) + GetInnerOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return Wnd::HitTest(point -= GetInnerOffset()); }

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Offset(GetInnerOffset(), [&]() { Wnd::OnDraw(figure_queue, draw_region - GetInnerOffset()); });
	}
	virtual Rect GetRedrawRegion() override { return Wnd::GetRedrawRegion() + GetInnerOffset(); }
};


END_NAMESPACE(WndDesign)