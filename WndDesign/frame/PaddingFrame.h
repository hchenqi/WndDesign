#pragma once

#include "WndFrame.h"
#include "../geometry/margin.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class PaddingFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	PaddingFrame(Margin padding, child_ptr child) : WndFrame(std::move(child)), padding(padding) {}
private:
	Margin padding;
private:
	Vector GetChildOffset() const { return Vector(padding.left, padding.top); }
private:
	static Size ExtendSize(Size size, Margin margin) {
		int width = (int)size.width + margin.left + margin.right;
		int height = (int)size.height + margin.top + margin.bottom;
		return Size(width >= 0 ? width : 0, height >= 0 ? height : 0);
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return ExtendSize(UpdateChildSizeRef(child, ExtendSize(size_ref, -padding)), padding); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { SizeUpdated(ExtendSize(child_size, padding)); }
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { Redraw(redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { return DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region); }
};


END_NAMESPACE(WndDesign)