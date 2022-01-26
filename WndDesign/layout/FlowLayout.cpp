#include "FlowLayout.h"

#include <algorithm>


BEGIN_NAMESPACE(WndDesign)

#pragma warning (disable : 26451)  // Arithmetic overflow : Using operator '+' on a 4 byte value and then casting the result to a 8 byte value.
#pragma warning (disable : 4267)  // conversion from 'size_t' to 'WndDesign::uint', possible loss of data


FlowLayout::FlowLayout(uint row_height, uint column_gap, uint row_gap) :
	row_height(row_height), column_gap(column_gap), row_gap(row_gap), row_list({ 0 }) {
}

Rect FlowLayout::GetChildRegion(WndObject& child) const {
	auto [row, column] = GetChildData(child); const ChildInfo& info = child_list[row_list[row] + column];
	return Rect(Point((int)info.offset, (int)GetRowOffset(row)), Size(info.width, row_height));
}

uint FlowLayout::GetRowOffset(uint row) const { return row * (row_height + row_gap); }

uint FlowLayout::GetRowNumber() const { return row_list.size() - 1; }

uint FlowLayout::HitTestRow(uint y) const { return y / (row_height + row_gap); }

auto FlowLayout::HitTestColumn(row_index row, uint x) const {
	static auto cmp = [](const ChildInfo& item, uint offset) { return offset >= item.offset; };
	return std::lower_bound(child_list.begin() + row_list[row], child_list.begin() + row_list[row + 1], x, cmp) - 1;
}

void FlowLayout::AppendChild(child_ptr child) {
	RegisterChild(child);
	child_index child_index = child_list.size();
	ChildInfo& info = child_list.emplace_back(std::move(child));
	info.width = UpdateChildSizeRef(info.child, Size(length_min, row_height)).width;
	if (UpdateLayout(child_index)) { SizeUpdated(); }
}

bool FlowLayout::UpdateLayout(child_index child_index) {
	uint row = 0, column = 0, offset = 0;
	if (child_index > 0) {
		ChildInfo& prev = child_list[child_index - 1]; std::tie(row, column) = GetChildData(prev.child);
		column++; offset = prev.offset + prev.width + column_gap;
	}
	uint row_begin = row;
	row_list.erase(row_list.begin() + row + 1, row_list.end());
	for (; child_index < child_list.size(); ++child_index) {
		ChildInfo& info = child_list[child_index];
		if (offset > 0 && offset + info.width > size.width) { row_list.push_back(child_index); row++; column = 0; offset = 0; }
		info.offset = offset;
		SetChildData(info.child, { row, column });
		offset += info.width + column_gap; column++;
	}
	row_list.push_back((uint)child_list.size());
	uint height = child_list.empty() ? 0 : GetRowOffset(row) + row_height;
	if (size.height == height) { redraw_region = Rect(Point(0, (int)GetRowOffset(row_begin)), size_max); Redraw(); return false; }
	size.height = height; return true;
}

void FlowLayout::OnSizeRefUpdate(Size size_ref) {
	if (size.width != size_ref.width) {
		size.width = size_ref.width;
		UpdateLayout(0);
	}
}

void FlowLayout::OnChildSizeUpdate(WndObject& child, Size child_size) {
	auto [row, column] = GetChildData(child);
	child_index child_index = row_list[row] + column;
	ChildInfo& info = child_list[child_index];
	if (info.width != child_size.width) {
		info.width = child_size.width;
		if (UpdateLayout(child_index)) { SizeUpdated(); }
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

void FlowLayout::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	redraw_region = child_region.Intersect(child_redraw_region + (child_region.point - point_zero));
	if (!redraw_region.IsEmpty()) { Redraw(); }
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