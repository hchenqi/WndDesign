#include "SplitLayout.h"


BEGIN_NAMESPACE(WndDesign)


SplitLayout<Vertical, First>::SplitLayout(child_ptr_first first, child_ptr_second second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first); RegisterChild(this->second);
}

void SplitLayout<Vertical, First>::UpdateLayout() {
	length_second = max(0.0f, size.height - length_first);
	UpdateChildSizeRef(second, GetRegionSecond().size);
}

Size SplitLayout<Vertical, First>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) {
		size = size_ref;
		length_first = UpdateChildSizeRef(first, Size(size.width, length_min)).height;
		UpdateLayout();
	}
	return size;
}

void SplitLayout<Vertical, First>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == first.get() && length_first != child_size.height) {
		length_first = child_size.height;
		UpdateLayout(); Redraw(GetRegionSecond());
	}
}

ref_ptr<WndObject> SplitLayout<Vertical, First>::HitTest(Point& point) {
	if (point.y < length_first) { return first; }
	point.y -= length_first;
	if (point.y < length_second) { return second; }
	return nullptr;
}

Transform SplitLayout<Vertical, First>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitLayout<Vertical, First>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitLayout<Vertical, First>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


SplitLayout<Vertical, Second>::SplitLayout(child_ptr_first first, child_ptr_second second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first); RegisterChild(this->second);
}

void SplitLayout<Vertical, Second>::UpdateLayout() {
	length_first = max(0.0f, size.height - length_second);
	UpdateChildSizeRef(first, GetRegionFirst().size);
}

Size SplitLayout<Vertical, Second>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) {
		size = size_ref;
		length_second = UpdateChildSizeRef(second, Size(size.width, length_min)).height;
		UpdateLayout();
	}
	return size;
}

void SplitLayout<Vertical, Second>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == second.get() && length_second != child_size.height) {
		length_second = child_size.height;
		UpdateLayout(); Redraw(GetRegionFirst());
	}
}

ref_ptr<WndObject> SplitLayout<Vertical, Second>::HitTest(Point& point) {
	if (point.y < length_first) { return first; }
	point.y -= length_first;
	if (point.y < length_second) { return second; }
	return nullptr;
}

Transform SplitLayout<Vertical, Second>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitLayout<Vertical, Second>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitLayout<Vertical, Second>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


SplitLayout<Horizontal, First>::SplitLayout(child_ptr_first first, child_ptr_second second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first); RegisterChild(this->second);
}

void SplitLayout<Horizontal, First>::UpdateLayout() {
	length_second = max(0.0f, size.width - length_first);
	UpdateChildSizeRef(second, GetRegionSecond().size);
}

Size SplitLayout<Horizontal, First>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) {
		size = size_ref;
		length_first = UpdateChildSizeRef(first, Size(length_min, size.height)).width;
		UpdateLayout();
	}
	return size;
}

void SplitLayout<Horizontal, First>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == first.get() && length_first != child_size.width) {
		length_first = child_size.width;
		UpdateLayout(); Redraw(GetRegionSecond());
	}
}

ref_ptr<WndObject> SplitLayout<Horizontal, First>::HitTest(Point& point) {
	if (point.x < length_first) { return first; }
	point.x -= length_first;
	if (point.x < length_second) { return second; }
	return nullptr;
}

Transform SplitLayout<Horizontal, First>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitLayout<Horizontal, First>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitLayout<Horizontal, First>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


SplitLayout<Horizontal, Second>::SplitLayout(child_ptr_first first, child_ptr_second second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first); RegisterChild(this->second);
}

void SplitLayout<Horizontal, Second>::UpdateLayout() {
	length_first = max(0.0f, size.width - length_second);
	UpdateChildSizeRef(first, GetRegionFirst().size);
}

Size SplitLayout<Horizontal, Second>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) {
		size = size_ref;
		length_second = UpdateChildSizeRef(second, Size(length_min, size.height)).width;
		UpdateLayout();
	}
	return size;
}

void SplitLayout<Horizontal, Second>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == second.get() && length_second != child_size.width) {
		length_second = child_size.width;
		UpdateLayout(); Redraw(GetRegionFirst());
	}
}

ref_ptr<WndObject> SplitLayout<Horizontal, Second>::HitTest(Point& point) {
	if (point.x < length_first) { return first; }
	point.x -= length_first;
	if (point.x < length_second) { return second; }
	return nullptr;
}

Transform SplitLayout<Horizontal, Second>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitLayout<Horizontal, Second>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitLayout<Horizontal, Second>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


END_NAMESPACE(WndDesign)