#pragma once

#include "wrapper.h"
#include "../geometry/helper.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd>
class Border : public Wnd {
public:
	using Wnd::Wnd;

	// style
protected:
	struct Style {
	public:
		uint _width = 0;
		uint _radius = 0;
		Color _color = color_transparent;
	public:
		constexpr Style& width(uint width) { _width = width; return *this; }
		constexpr Style& radius(uint radius) { _radius = radius; return *this; }
		constexpr Style& color(Color color) { _color = color; return *this; }
	}border;

	// layout
protected:
	Size size;
	Size inner_size;
protected:
	Vector GetInnerOffset() const { return Vector(border._width, border._width); }
	Rect GetInnerRegion() const { return Rect(Point(border._width, border._width), inner_size); }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { Wnd::OnSizeRefUpdate(Extend(size_ref, -(int)border._width)); }
	virtual Size GetSize() override { return size = Extend(inner_size = Wnd::GetSize(), (int)border._width); }
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return Wnd::GetChildOffset(child) + GetInnerOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (PointInRoundedRectangle(point, GetInnerRegion(), border._radius)) {
			Point child_point = point -  GetInnerOffset();
			ref_ptr<WndObject> child = Wnd::HitTest(child_point);
			return child == this ? this : (point = child_point, child);
		}
		return this;
	}

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Offset(GetInnerOffset(), [&]() { Wnd::OnDraw(figure_queue, draw_region - GetInnerOffset()); });
		if (border._width > 0 && border._color.IsVisible()) {
			if (border._radius > 0) {
				figure_queue.add(point_zero, new RoundedRectangle(size, border._radius, (float)border._width, border._color));
			} else {
				figure_queue.add(point_zero, new Rectangle(size, (float)border._width, border._color));
			}
		}
	}
	virtual Rect GetRedrawRegion() override { return Wnd::GetRedrawRegion() + GetInnerOffset(); }

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (PointInRoundedRectangle(msg.point, GetInnerRegion(), border._radius)) {
			msg.point -= GetInnerOffset(); Wnd::OnMouseMsg(msg);
		}
	}
};


END_NAMESPACE(WndDesign)