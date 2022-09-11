#pragma once

#include "WndFrame.h"
#include "../style/border_style.h"
#include "../geometry/helper.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class BorderFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;

public:
	BorderFrame(Border border, child_ptr child) : WndFrame(std::move(child)), border(border) {}

	// style
protected:
	Border border;

	// layout
protected:
	Size size;
	Size child_size;
protected:
	Vector GetChildOffset() const { return Vector(border._width, border._width); }
	Rect GetChildRegion() const { return Rect(Point(border._width, border._width), child_size); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size = Extend(child_size = UpdateChildSizeRef(child, Extend(size_ref, -border._width)), border._width); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(size = Extend(this->child_size = child_size, border._width)); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (PointInRoundedRectangle(point, GetChildRegion(), border._radius)) { point -= GetChildOffset(); return child; }
		return this;
	}
	virtual Transform GetChildTransform(WndObject& child) const override { return GetChildOffset(); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Redraw(child_redraw_region + GetChildOffset());
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
		if (border._width > 0.0f && border._color.IsVisible()) {
			if (border._radius > 0.0f) {
				figure_queue.add(point_zero, new RoundedRectangle(size, border._radius, border._width, border._color));
			} else {
				figure_queue.add(point_zero, new Rectangle(size, border._width, border._color));
			}
		}
	}
};


template<class T>
BorderFrame(Border, T)->BorderFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)