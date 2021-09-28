#pragma once

#include "../window/wnd_traits.h"
#include "../common/array_vector_selector.h"

#include <algorithm>


BEGIN_NAMESPACE(WndDesign)


template<class Direction, size_t extent = dynamic_extent>
class ListLayout;


template<size_t extent>
class ListLayout<Vertical, extent> : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;
public:
	template<class ...child_types>
	ListLayout(uint row_gap, child_types... child_args) :
		child_list(array_vector_selector<ChildInfo, extent>::construct(std::forward<child_types>(child_args)...)), row_gap(row_gap) {
		uint row = 0; for (auto& info : child_list) { RegisterChild(info.child); SetChildData(info.child, row++); }
	}
private:
	struct ChildInfo {
		child_ptr child;
		uint y = 0;
		uint height = 0;
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	};
	array_vector_selector_t<ChildInfo, extent> child_list;
private:
	void SetChildData(WndObject& child, uint64 index) { WndObject::SetChildData<uint64>(child, index); }
	uint64 GetChildData(WndObject& child) const { return WndObject::GetChildData<uint64>(child); }
public:
	template<class = std::enable_if_t<extent == dynamic_extent>>
	void AppendChild(child_ptr child) {
		RegisterChild(child);
		SetChildData(child, child_list.size());
		ChildInfo& info = child_list.emplace_back(std::move(child));
		info.y = size.height;
		info.height = UpdateChildSizeRef(info.child, Size(size.width, length_min)).height;
		size.height += info.height;
		SizeUpdated(size);
	}
private:
	Size size;
	uint row_gap;
private:
	Rect GetChildRegion(WndObject& child) const {
		uint64 index = GetChildData(child);
		return Rect(Point(0, (int)child_list[index].y), Size(size.width, child_list[index].height));
	}
	auto HitTestRow(uint y) const {
		static auto cmp = [](const ChildInfo& row, uint y) { return y >= row.y; };
		return std::lower_bound(child_list.begin(), child_list.end(), y, cmp) - 1;
	}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			for (auto& info : child_list) {
				info.y = size.height;
				info.height = UpdateChildSizeRef(*info.child, Size(size.width, length_min)).height;
				size.height += info.height + row_gap;
			}
			size.height -= child_list.empty() ? 0 : row_gap;
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		uint64 index = GetChildData(child);
		if (child_list[index].height != child_size.height) {
			child_list[index].height = child_size.height;
			size.height = child_list[index].y + child_list[index].height;
			for (index++; index < child_list.size(); index++) {
				size.height += row_gap;
				child_list[index].y = size.height;
				size.height += child_list[index].height;
			}
			SizeUpdated(size);
		}
	}
private:
	virtual Vector GetChildOffset(WndObject& child) override {
		return GetChildRegion(child).point - point_zero;
	}
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.y >= size.height) { return nullptr; }
		auto it = HitTestRow((uint)point.y); point.y -= (int)it->y;
		if ((uint)point.y >= it->height) { return nullptr; }
		return it->child;
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		Rect child_region = GetChildRegion(child);
		redraw_region = child_region.Intersect(redraw_region + (child_region.point - point_zero));
		if (!redraw_region.IsEmpty()) { Redraw(redraw_region); }
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
		auto it_begin = HitTestRow(draw_region.top());
		auto it_end = HitTestRow(draw_region.bottom() - 1);
		for (auto it = it_begin; it <= it_end; ++it) {
			DrawChild(it->child, Point(0, (int)it->y), figure_queue, draw_region);
		}
	}
};


END_NAMESPACE(WndDesign)