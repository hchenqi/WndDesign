#include "BarLayout.h"


BEGIN_NAMESPACE(WndDesign)


BarLayout::BarLayout(float height, child_type left, child_type right, child_type center) :
	left(std::move(left)), right(std::move(right)), center(std::move(center)), size(0.0f, height) {
	RegisterChild(this->left); RegisterChild(this->right); RegisterChild(this->center);
	width_left = UpdateChildSizeRef(this->left, size).width;
	width_right = UpdateChildSizeRef(this->right, size).width;
	width_center = UpdateChildSizeRef(this->center, size).width;
}

Transform BarLayout::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

Size BarLayout::OnSizeRefUpdate(Size size_ref) { size.width = size_ref.width; return size; }

void BarLayout::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == center.get()) {
		if (width_center != child_size.width) {
			Rect region_old = GetRegionCenter();
			width_center = child_size.width;
			Redraw(region_old.Union(GetRegionCenter()));
		}
	} else if (&child == right.get()) {
		if (width_right != child_size.width) {
			Rect region_old = GetRegionRight();
			width_right = child_size.width;
			Redraw(region_old.Union(GetRegionRight()));
		}
	} else {
		if (width_left != child_size.width) {
			Rect region_old = GetRegionLeft();
			width_left = child_size.width;
			Redraw(region_old.Union(GetRegionLeft()));
		}
	}
}

void BarLayout::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void BarLayout::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(left, GetRegionLeft(), figure_queue, draw_region);
	DrawChild(right, GetRegionRight(), figure_queue, draw_region);
	DrawChild(center, GetRegionCenter(), figure_queue, draw_region);
}

ref_ptr<WndObject> BarLayout::HitTest(MouseMsg& msg) {
	using pair = std::pair<WndObject&, Rect>;
	for (auto [child, child_region] : { pair{ center, GetRegionCenter() }, pair{ right, GetRegionRight() }, pair{ left, GetRegionLeft() } }) {
		if (child_region.Contains(msg.point)) { msg.point -= child_region.point - point_zero; return &child; }
	}
	return this;
}


END_NAMESPACE(WndDesign)