#pragma once

#include "ListLayout.h"

#include <algorithm>


BEGIN_NAMESPACE(WndDesign)


void ListLayout<Vertical>::UpdateIndex(size_t begin) {
	for (size_t index = begin; index < child_list.size(); ++index) {
		SetChildIndex(child_list[index].child, index);
	}
}

void ListLayout<Vertical>::InsertChild(size_t index, child_ptr child) {
	RegisterChild(child);
	if (index > child_list.size()) { index = child_list.size(); }
	auto it = child_list.emplace(child_list.begin() + index, std::move(child));
	UpdateIndex(index);
	it->length = UpdateChildSizeRef(it->child, Size(size.width, length_min)).height;
	UpdateLayout(index);
}

void ListLayout<Vertical>::InsertChild(size_t index, std::vector<child_ptr> children) {
	for (auto& child : children) { RegisterChild(child); }
	if (index > child_list.size()) { index = child_list.size(); }
	auto it = child_list.insert(child_list.begin() + index, std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
	UpdateIndex(index);
	for (auto it_end = it + children.size(); it != it_end; ++it) {
		it->length = UpdateChildSizeRef(it->child, Size(size.width, length_min)).height;
	}
	UpdateLayout(index);
}

void ListLayout<Vertical>::EraseChild(size_t begin, size_t count) {
	if (begin > child_list.size() || count == 0) { return; }
	size_t end = begin + count; if (end > child_list.size()) { end = child_list.size(); }
	child_list.erase(child_list.begin() + begin, child_list.begin() + end);
	UpdateIndex(begin); UpdateLayout(begin);
}

ListLayout<Vertical>::child_iter ListLayout<Vertical>::HitTestItem(float offset) {
	static auto cmp = [](const ChildInfo& item, float offset) { return offset >= item.offset; };
	return std::lower_bound(child_list.begin(), child_list.end(), offset, cmp) - 1;
}

void ListLayout<Vertical>::UpdateLayout(size_t index) {
	size.height = index == 0 ? (child_list.size() == 0 ? 0.0f : -gap) : child_list[index - 1].EndOffset();
	for (index; index < child_list.size(); index++) {
		size.height += gap;
		child_list[index].offset = size.height;
		size.height += child_list[index].length;
	}
	SizeUpdated(size);
}

Size ListLayout<Vertical>::OnSizeRefUpdate(Size size_ref) {
	if (size.width != size_ref.width) {
		size.width = size_ref.width;
		size.height = 0.0f;
		for (auto& info : child_list) {
			info.offset = size.height;
			info.length = UpdateChildSizeRef(*info.child, Size(size.width, length_min)).height;
			size.height += info.length + gap;
		}
		size.height -= child_list.empty() ? 0.0f : gap;
	}
	return size;
}

void ListLayout<Vertical>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	size_t index = GetChildIndex(child);
	if (child_list[index].length != child_size.height) {
		child_list[index].length = child_size.height;
		UpdateLayout(index + 1);
	}
}

ref_ptr<WndObject> ListLayout<Vertical>::HitTest(Point& point) {
	float offset = point.y;
	if (offset < 0.0f || offset >= size.height) { return nullptr; }
	auto it = HitTestItem(offset); offset -= it->offset;
	if (offset >= it->length) { return this; }
	point.y = offset; return it->child;
}

Transform ListLayout<Vertical>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void ListLayout<Vertical>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void ListLayout<Vertical>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	auto it_begin = HitTestItem(draw_region.top());
	auto it_end = HitTestItem(ceilf(draw_region.bottom()) - 1.0f);
	for (auto it = it_begin; it <= it_end; ++it) {
		Rect child_region(Point(0.0f, it->offset), Size(size.width, it->length));
		DrawChild(it->child, child_region, figure_queue, draw_region);
	}
}


void ListLayout<Horizontal>::UpdateIndex(size_t begin) {
	for (size_t index = begin; index < child_list.size(); ++index) {
		SetChildIndex(child_list[index].child, index);
	}
}

void ListLayout<Horizontal>::InsertChild(size_t index, child_ptr child) {
	RegisterChild(child);
	if (index > child_list.size()) { index = child_list.size(); }
	auto it = child_list.emplace(child_list.begin() + index, std::move(child));
	UpdateIndex(index);
	it->length = UpdateChildSizeRef(it->child, Size(length_min, size.height)).width;
	UpdateLayout(index);
}

void ListLayout<Horizontal>::InsertChild(size_t index, std::vector<child_ptr> children) {
	for (auto& child : children) { RegisterChild(child); }
	if (index > child_list.size()) { index = child_list.size(); }
	auto it = child_list.insert(child_list.begin() + index, std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
	UpdateIndex(index);
	for (auto it_end = it + children.size(); it != it_end; ++it) {
		it->length = UpdateChildSizeRef(it->child, Size(length_min, size.height)).width;
	}
	UpdateLayout(index);
}

void ListLayout<Horizontal>::EraseChild(size_t begin, size_t count) {
	if (begin > child_list.size() || count == 0) { return; }
	size_t end = begin + count; if (end > child_list.size()) { end = child_list.size(); }
	child_list.erase(child_list.begin() + begin, child_list.begin() + end);
	UpdateIndex(begin); UpdateLayout(begin);
}

ListLayout<Horizontal>::child_iter ListLayout<Horizontal>::HitTestItem(float offset) {
	static auto cmp = [](const ChildInfo& item, float offset) { return offset >= item.offset; };
	return std::lower_bound(child_list.begin(), child_list.end(), offset, cmp) - 1;
}

void ListLayout<Horizontal>::UpdateLayout(size_t index) {
	size.width = index == 0 ? (child_list.size() == 0 ? 0.0f : -gap) : child_list[index - 1].EndOffset();
	for (index; index < child_list.size(); index++) {
		size.width += gap;
		child_list[index].offset = size.width;
		size.width += child_list[index].length;
	}
	SizeUpdated(size);
}

Size ListLayout<Horizontal>::OnSizeRefUpdate(Size size_ref) {
	if (size.height != size_ref.height) {
		size.height = size_ref.height;
		size.width = 0.0f;
		for (auto& info : child_list) {
			info.offset = size.width;
			info.length = UpdateChildSizeRef(*info.child, Size(length_min, size.height)).width;
			size.width += info.length + gap;
		}
		size.width -= child_list.empty() ? 0.0f : gap;
	}
	return size;
}

void ListLayout<Horizontal>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	size_t index = GetChildIndex(child);
	if (child_list[index].length != child_size.width) {
		child_list[index].length = child_size.width;
		UpdateLayout(index + 1);
	}
}

ref_ptr<WndObject> ListLayout<Horizontal>::HitTest(Point& point) {
	float offset = point.x;
	if (offset < 0.0f || offset >= size.width) { return nullptr; }
	auto it = HitTestItem(offset); offset -= it->offset;
	if (offset >= it->length) { return this; }
	point.x = offset; return it->child;
}

Transform ListLayout<Horizontal>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void ListLayout<Horizontal>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void ListLayout<Horizontal>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	auto it_begin = HitTestItem(draw_region.left());
	auto it_end = HitTestItem(ceilf(draw_region.right()) - 1.0f);
	for (auto it = it_begin; it <= it_end; ++it) {
		Rect child_region(Point(it->offset, 0.0f), Size(it->length, size.height));
		DrawChild(it->child, child_region, figure_queue, draw_region);
	}
}


END_NAMESPACE(WndDesign)