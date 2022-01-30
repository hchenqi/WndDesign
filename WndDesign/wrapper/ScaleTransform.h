#pragma once

#include "wrapper.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd>
class ScaleTransform : public Wnd {
public:
	using Wnd::Wnd;
	using Base = ScaleTransform;

	// style
protected:
	float scale_x = 1.0f;
	float scale_y = 1.0f;

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { Wnd::OnSizeRefUpdate(Scale(size_ref, 1.0f / scale_x, 1.0f / scale_y)); }
	virtual Size GetSize() override { return Scale(Wnd::GetSize(), scale_x, scale_y); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		Point child_point = Scale(point, 1.0f / scale_x, 1.0f / scale_y); ref_ptr<WndObject> child = Wnd::HitTest(child_point);
		return child == this ? this : (point = child_point, child);
	}
	virtual Transform GetChildTransform(WndObject& child) const override { return Wnd::GetChildTransform(child) * Transform::Scale(scale_x, scale_y); }

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Group(Transform::Scale(scale_x, scale_y), region_infinite, [&]() {
			Wnd::OnDraw(figure_queue, Scale(draw_region, 1.0f / scale_x, 1.0f / scale_y));
		});
	}
	virtual Rect GetRedrawRegion() override { return Scale(Wnd::GetRedrawRegion(), scale_x, scale_y); }

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override { msg.point = Scale(msg.point, 1.0f / scale_x, 1.0f / scale_y); Wnd::OnMouseMsg(msg); }
};


END_NAMESPACE(WndDesign)