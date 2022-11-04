#pragma once

#include "WndFrame.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class ScaleFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;

public:
	ScaleFrame(Scale scale, child_ptr child) : WndFrame(std::move(child)), scale(scale) {}

	// style
protected:
	Scale scale;

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref * scale.Invert()) * scale; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(child_size * scale); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point *= scale.Invert(); return child; }
	virtual Transform GetChildTransform(WndObject& child) const override { return scale; }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Redraw(child_redraw_region * scale);
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Group(scale, region_infinite, [&]() {
			DrawChild(child, point_zero, figure_queue, draw_region * scale.Invert());
		});
	}
};


template<class T>
ScaleFrame(Scale, T)->ScaleFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)