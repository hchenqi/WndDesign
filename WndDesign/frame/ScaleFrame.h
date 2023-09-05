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
protected:
	void SetScale(Scale scale) {
		if (this->scale != scale) {
			this->scale = scale;
			SizeUpdated(UpdateChildSizeRef(child, size_ref * scale.Invert()) * scale);
			Redraw(region_infinite);
		}
	}

	// layout
protected:
	Size size_ref;
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return scale; }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, (this->size_ref = size_ref) * scale.Invert()) * scale; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(child_size * scale); }

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

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override { msg.point *= scale.Invert(); return child; }
};


template<class T>
ScaleFrame(Scale, T) -> ScaleFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)