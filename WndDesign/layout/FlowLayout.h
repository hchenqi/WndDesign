#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class FlowLayout : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;
public:
	FlowLayout(uint row_height, uint column_gap, uint row_gap) :
		row_height(row_height), column_gap(column_gap), row_gap(row_gap), row_list({ 0 }) {
	}
private:
	uint row_height;
	uint column_gap;
	uint row_gap;
private:
	Size size;
private:
	struct ChildInfo {
		child_ptr child;
		uint width = 0;
		uint offset = 0;
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	};
	std::vector<ChildInfo> child_list;
private:
	using child_index = uint;
	std::vector<child_index> row_list;
private:
	using row_index = uint; using column_index = uint;
	using ChildData = std::pair<row_index, column_index>;
private:
	void SetChildData(WndObject& child, ChildData data) { WndObject::SetChildData<ChildData>(child, data); }
	ChildData GetChildData(WndObject& child) const { return WndObject::GetChildData<ChildData>(child); }
private:
	Rect GetChildRegion(WndObject& child) const {
		auto [row, column] = GetChildData(child); const ChildInfo& info = child_list[(size_t)row_list[row] + column];
		return Rect(Point((int)info.offset, (int)GetRowOffset(row)), Size(info.width, row_height));
	}
	uint GetRowOffset(uint row) const { return row * (row_height + row_gap); }
	uint GetRowNumber() const { return (uint)row_list.size() - 1; }
	uint HitTestRow(uint y) const { return y / (row_height + row_gap); }
	auto HitTestColumn(row_index row, uint x) const;
public:
	void AppendChild(child_ptr child);
private:
	void UpdateLayout(row_index row, column_index column, uint offset);
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
private:
	virtual Vector GetChildOffset(WndObject& child) override;
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)