#pragma once

#include "WndFrame.h"
#include "../style/border_style.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class InnerBorderFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;

public:
	InnerBorderFrame(Border border, child_ptr child) : WndFrame(std::move(child)), border(border) {}

	// style
protected:
	Border border;

	// layout
protected:
	Size size;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size = UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(size = child_size); }

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero, figure_queue, draw_region);
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
InnerBorderFrame(Border, T)->InnerBorderFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)