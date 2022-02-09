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
	BarLayout(float height, child_ptr left, child_ptr right, child_ptr center) :
		left(std::move(left)), right(std::move(right)), center(std::move(center)), size(0.0f, height) {
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
	float width_left = 0.0f;
	float width_right = 0.0f;
	float width_center = 0.0f;
protected:
	Rect GetRegionLeft() const { return Rect(Point(), Size(width_left, size.height)); }
	Rect GetRegionRight() const { return Rect(Point(size.width - width_right, 0.0f), Size(width_right, size.height)); }
	Rect GetRegionCenter() const { return Rect(Point((size.width - width_center) / 2, 0.0f), Size(width_center, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == center.get() ? GetRegionCenter() : &child == right.get() ? GetRegionRight() : GetRegionLeft(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; return size; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == center.get()) {
			if (width_center != child_size.width) { width_center = child_size.width; }
		} else if (&child == right.get()) {
			if (width_right != child_size.width) { width_right = child_size.width; }
		} else {
			if (width_left != child_size.width) { width_left = child_size.width; }
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		using pair = std::pair<WndObject&, Rect>;
		for (auto [child, child_region] : { pair{center, GetRegionCenter()}, pair{right, GetRegionRight()}, pair{left, GetRegionLeft()} }) {
			if (child_region.Contains(point)) { point -= child_region.point - point_zero; return &child; }
		}
		return this;
	}
	virtual Transform GetChildTransform(WndObject& child) const override {
		return GetChildRegion(child).point - point_zero;
	}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(left, GetRegionLeft(), figure_queue, draw_region);
		DrawChild(right, GetRegionRight(), figure_queue, draw_region);
		DrawChild(center, GetRegionCenter(), figure_queue, draw_region);
	}
};


END_NAMESPACE(WndDesign)