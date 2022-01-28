#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class BarLayout;


template<>
class BarLayout<Horizontal> : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	BarLayout(uint height, child_ptr left, child_ptr right, child_ptr center) :
		left(std::move(left)), right(std::move(right)), center(std::move(center)), size(0, height) {
		RegisterChild(this->left); RegisterChild(this->right); RegisterChild(this->center);
		width_left = UpdateChildSizeRef(this->left, size).width;
		width_right = UpdateChildSizeRef(this->right, size).width;
		width_center = UpdateChildSizeRef(this->center, size).width;
	}

	// child
protected:
	child_ptr left;
	child_ptr right;
	child_ptr center;

	// layout
private:
	Size size;
	uint width_left = 0;
	uint width_right = 0;
	uint width_center = 0;
protected:
	Rect GetRegionLeft() const { return Rect(Point(0, 0), Size(width_left, size.height)); }
	Rect GetRegionRight() const { return Rect(Point((int)size.width - (int)width_right, 0), Size(width_right, size.height)); }
	Rect GetRegionCenter() const { return Rect(Point(((int)size.width - (int)width_center) / 2, 0), Size(width_center, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == center.get() ? GetRegionCenter() : &child == right.get() ? GetRegionRight() : GetRegionLeft(); }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == center.get()) {
			if (width_center != child_size.width) { width_center = child_size.width; }
		} else if (&child == right.get()) {
			if (width_right != child_size.width) { width_right = child_size.width; }
		} else {
			if (width_left != child_size.width) { width_left = child_size.width; }
		}
	}
	virtual Size GetSize() override { return size; }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		using pair = std::pair<WndObject&, Rect>;
		for (auto [child, child_region] : { pair{center, GetRegionCenter()}, pair{right, GetRegionRight()}, pair{left, GetRegionLeft()} }) {
			if (child_region.Contains(point)) { point -= child_region.point - point_zero; return &child; }
		}
		return this;
	}
	virtual Transform GetChildTransform(WndObject& child) override {
		return GetChildRegion(child).point - point_zero;
	}

	// paint
protected:
	Rect redraw_region;
protected:
	virtual Rect GetRedrawRegion() override { return redraw_region; }
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		redraw_region = child_region.Intersect(child_redraw_region + (child_region.point - point_zero));
		if (!redraw_region.IsEmpty()) { Redraw(); }
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(left, GetRegionLeft(), figure_queue, draw_region);
		DrawChild(right, GetRegionRight(), figure_queue, draw_region);
		DrawChild(center, GetRegionCenter(), figure_queue, draw_region);
	}
};


END_NAMESPACE(WndDesign)