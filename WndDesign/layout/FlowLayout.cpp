#include "FlowLayout.h"

#include <algorithm>


BEGIN_NAMESPACE(WndDesign)


#pragma warning (disable : 26451)  // Arithmetic overflow : Using operator '+' on a 4 byte value and then casting the result to a 8 byte value.

auto FlowLayout::HitTestColumn(row_index row, uint x) const {
	static auto cmp = [](const ChildInfo& item, uint offset) { return offset >= item.offset; };
	return std::lower_bound(child_list.begin() + row_list[row], child_list.begin() + row_list[row + 1], x, cmp) - 1;
}

void FlowLayout::AppendChild(child_ptr child) {
	RegisterChild(child);
	uint row = 0, column = 0, offset = 0;
	if (!child_list.empty()) {
		ChildInfo& child_back = child_list.back(); std::tie(row, column) = GetChildData(child_back.child);
		column++; offset = child_back.offset + child_back.width + column_gap;
	}
	ChildInfo& info = child_list.emplace_back(std::move(child));
	info.width = UpdateChildSizeRef(info.child, Size(length_min, row_height)).width;
	Size size_old = size; UpdateLayout(row, column, offset);
	if (size != size_old) { SizeUpdated(size); } else { Redraw(Rect(Point(0, (int)GetRowOffset(row)), size_max)); }
}

void FlowLayout::UpdateLayout(row_index row, column_index column, uint offset) {
	row_list.erase(row_list.begin() + row + 1, row_list.end());
	for (child_index child_index = row_list[row] + column; child_index < child_list.size(); ++child_index) {
		ChildInfo& info = child_list[child_index];
		if (offset > 0 && offset + info.width > size.width) { row_list.push_back(child_index); row++; column = 0; offset = 0; }
		info.offset = offset;
		SetChildData(info.child, { row, column });
		offset += info.width + column_gap; column++;
	}
	row_list.push_back((uint)child_list.size());
	size.height = child_list.empty() ? 0 : GetRowOffset(row) + row_height;
}

Size FlowLayout::OnSizeRefUpdate(Size size_ref) {
	if (size.width != size_ref.width) {
		size.width = size_ref.width; size.height = 0;
		UpdateLayout(0, 0, 0);
	}
	return size;
}

void FlowLayout::OnChildSizeUpdate(WndObject& child, Size child_size) {
	auto [row, column] = GetChildData(child);
	ChildInfo& info = child_list[row_list[row] + column];
	if (info.width != child_size.width) {
		info.width = child_size.width;
		Size size_old = size; UpdateLayout(row, column, info.offset);
		if (size != size_old) { SizeUpdated(size); } else { Redraw(Rect(Point(0, (int)GetRowOffset(row)), size_max)); }
	}
}

Vector FlowLayout::GetChildOffset(WndObject& child) {
	return GetChildRegion(child).point - point_zero;
}

ref_ptr<WndObject> FlowLayout::HitTest(Point& point) {
	if ((uint)point.x >= size.width || (uint)point.y >= size.height) { return nullptr; }
	row_index row = HitTestRow(point.y); point.y -= GetRowOffset(row); if ((uint)point.y >= row_height) { return this; }
	auto it = HitTestColumn(row, point.x); point.x -= (int)it->offset; if ((uint)point.x >= it->width) { return this; }
	return it->child;
}

void FlowLayout::OnChildRedraw(WndObject& child, Rect redraw_region) {
	Rect child_region = GetChildRegion(child);
	redraw_region = child_region.Intersect(redraw_region + (child_region.point - point_zero));
	if (!redraw_region.IsEmpty()) { Redraw(redraw_region); }
}

void FlowLayout::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	row_index row_begin = HitTestRow(draw_region.top()), row_end = HitTestRow(draw_region.bottom() - 1);
	for (row_index row = row_begin; row <= row_end; ++row) {
		uint row_offset = GetRowOffset(row);
		auto it_begin = HitTestColumn(row, draw_region.left()), it_end = HitTestColumn(row, draw_region.right() - 1);
		for (auto it = it_begin; it <= it_end; ++it) {
			Rect child_region(Point((int)it->offset, (int)row_offset), Size(it->width, row_height));
			DrawChild(it->child, child_region, figure_queue, draw_region);
		}
	}
}


END_NAMESPACE(WndDesign)