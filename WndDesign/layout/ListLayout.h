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
	ListLayout(uint gap, child_types... child_args) :
		child_list(array_vector_selector<ChildInfo, extent>::construct(std::forward<child_types>(child_args)...)), gap(gap) {
		uint index = 0; for (auto& info : child_list) { RegisterChild(info.child); SetChildData(info.child, index++); }
	}
private:
	struct ChildInfo {
		child_ptr child;
		uint offset = 0;
		uint length = 0;
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
		info.offset = size.height;
		info.length = UpdateChildSizeRef(info.child, Size(size.width, length_min)).height;
		size.height += info.length;
		SizeUpdated(size);
	}
private:
	Size size;
	uint gap;
private:
	Rect GetChildRegion(WndObject& child) const {
		uint64 index = GetChildData(child);
		return Rect(Point(0, (int)child_list[index].offset), Size(size.width, child_list[index].length));
	}
	auto HitTestItem(uint offset) const {
		static auto cmp = [](const ChildInfo& item, uint offset) { return offset >= item.offset; };
		return std::lower_bound(child_list.begin(), child_list.end(), offset, cmp) - 1;
	}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			for (auto& info : child_list) {
				info.offset = size.height;
				info.length = UpdateChildSizeRef(*info.child, Size(size.width, length_min)).height;
				size.height += info.length + gap;
			}
			size.height -= child_list.empty() ? 0 : gap;
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		uint64 index = GetChildData(child);
		if (child_list[index].length != child_size.height) {
			child_list[index].length = child_size.height;
			size.height = child_list[index].offset + child_list[index].length;
			for (index++; index < child_list.size(); index++) {
				size.height += gap;
				child_list[index].offset = size.height;
				size.height += child_list[index].length;
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
		auto it = HitTestItem((uint)point.y); point.y -= (int)it->offset;
		if ((uint)point.y >= it->length) { return this; }
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
		auto it_begin = HitTestItem(draw_region.top());
		auto it_end = HitTestItem(draw_region.bottom() - 1);
		for (auto it = it_begin; it <= it_end; ++it) {
			DrawChild(it->child, Point(0, (int)it->offset), figure_queue, draw_region);
		}
	}
};


template<size_t extent>
class ListLayout<Horizontal, extent> : public WndType<Auto, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;
public:
	template<class ...child_types>
	ListLayout(uint gap, child_types... child_args) :
		child_list(array_vector_selector<ChildInfo, extent>::construct(std::forward<child_types>(child_args)...)), gap(gap) {
		uint index = 0; for (auto& info : child_list) { RegisterChild(info.child); SetChildData(info.child, index++); }
	}
private:
	struct ChildInfo {
		child_ptr child;
		uint offset = 0;
		uint length = 0;
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
		info.offset = size.width;
		info.length = UpdateChildSizeRef(info.child, Size(length_min, size.height)).width;
		size.width += info.length;
		SizeUpdated(size);
	}
private:
	Size size;
	uint gap;
private:
	Rect GetChildRegion(WndObject& child) const {
		uint64 index = GetChildData(child);
		return Rect(Point((int)child_list[index].offset, 0), Size(child_list[index].length, size.height));
	}
	auto HitTestItem(uint offset) const {
		static auto cmp = [](const ChildInfo& item, uint offset) { return offset >= item.offset; };
		return std::lower_bound(child_list.begin(), child_list.end(), offset, cmp) - 1;
	}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.height != size_ref.height) {
			size.height = size_ref.height;
			size.width = 0;
			for (auto& info : child_list) {
				info.offset = size.width;
				info.length = UpdateChildSizeRef(*info.child, Size(length_min, size.height)).width;
				size.width += info.length + gap;
			}
			size.width -= child_list.empty() ? 0 : gap;
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		uint64 index = GetChildData(child);
		if (child_list[index].length != child_size.width) {
			child_list[index].length = child_size.width;
			size.width = child_list[index].offset + child_list[index].length;
			for (index++; index < child_list.size(); index++) {
				size.width += gap;
				child_list[index].offset = size.width;
				size.width += child_list[index].length;
			}
			SizeUpdated(size);
		}
	}
private:
	virtual Vector GetChildOffset(WndObject& child) override {
		return GetChildRegion(child).point - point_zero;
	}
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)point.x >= size.width) { return nullptr; }
		auto it = HitTestItem((uint)point.x); point.x -= (int)it->offset;
		if ((uint)point.x >= it->length) { return this; }
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
		auto it_begin = HitTestItem(draw_region.left());
		auto it_end = HitTestItem(draw_region.right() - 1);
		for (auto it = it_begin; it <= it_end; ++it) {
			DrawChild(it->child, Point((int)it->offset, 0), figure_queue, draw_region);
		}
	}
};


END_NAMESPACE(WndDesign)