#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class BarLayout : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	BarLayout(float height, child_ptr left, child_ptr right, child_ptr center);

	// child
protected:
	child_ptr left;
	child_ptr right;
	child_ptr center;

	// layout
private:
	Size size;
	float width_left = 0.0f;
	float width_right = 0.0f;
	float width_center = 0.0f;
protected:
	Rect GetRegionLeft() const { return Rect(Point(), Size(width_left, size.height)); }
	Rect GetRegionRight() const { return Rect(Point(size.width - width_right, 0.0f), Size(width_right, size.height)); }
	Rect GetRegionCenter() const { return Rect(Point((size.width - width_center) / 2, 0.0f), Size(width_center, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == center.get() ? GetRegionCenter() : &child == right.get() ? GetRegionRight() : GetRegionLeft(); }
protected:
	virtual Transform GetChildTransform(WndObject& child) const override;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override;
};


END_NAMESPACE(WndDesign)