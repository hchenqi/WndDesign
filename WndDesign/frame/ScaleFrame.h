#pragma once

#include "WndFrame.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


struct ScaleRatio {
	float x;
	float y;

	constexpr ScaleRatio() : ScaleRatio(1.0f) {}
	constexpr ScaleRatio(float scale) : ScaleRatio(scale, scale) {}
	constexpr ScaleRatio(float x, float y) : x(x), y(y) {}
};


template<class WidthType, class HeightType>
class ScaleFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;

public:
	ScaleFrame(ScaleRatio scale, child_ptr child) : WndFrame(std::move(child)), scale(scale) {}

	// style
protected:
	ScaleRatio scale;

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return Scale(UpdateChildSizeRef(child, Scale(size_ref, 1.0f / scale.x, 1.0f / scale.y)), scale.x, scale.y); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(Scale(child_size, scale.x, scale.y)); }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point = Scale(point, 1.0f / scale.x, 1.0f / scale.y); return child; }
	virtual Transform GetChildTransform(WndObject& child) const override { return Transform::Scale(scale.x, scale.y); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Redraw(Scale(child_redraw_region, scale.x, scale.y));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.Group(Transform::Scale(scale.x, scale.y), region_infinite, [&]() {
			DrawChild(child, point_zero, figure_queue, Scale(draw_region, 1.0f / scale.x, 1.0f / scale.y));
		});
	}
};


template<class T>
ScaleFrame(ScaleRatio, T)->ScaleFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)