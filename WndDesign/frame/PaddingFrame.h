#pragma once

#include "WndFrame.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


using Padding = Margin;


template<class WidthType, class HeightType>
class PaddingFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;

public:
	PaddingFrame(Padding padding, child_ptr child) : WndFrame(std::move(child)), padding(padding) {}

	// style
protected:
	Padding padding;
private:
	Vector GetChildOffset() const { return Vector(padding.left, padding.top); }

	// layout
protected:
	Size size;
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return GetChildOffset(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size = Extend(UpdateChildSizeRef(child, Extend(size_ref, -padding)), padding); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(size = Extend(child_size, padding)); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { Redraw(child_redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { return DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region); }

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override { msg.point -= GetChildOffset(); return child; }
};


template<class T>
PaddingFrame(Padding, T) -> PaddingFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)