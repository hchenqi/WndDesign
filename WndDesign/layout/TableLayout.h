#pragma once

#include "../window/wnd_traits.h"
#include "../style/value_tag.h"
#include "../figure/shape.h"

#include <vector>
#include <algorithm>


BEGIN_NAMESPACE(WndDesign)


class TableLayout : public WndType<Relative, Relative> {
public:
	using child_ptr = child_ptr<Relative, Relative>;

public:
	struct TableSize {
		uint row_size;
		uint column_size;
	};

	struct TablePosition {
		uint row;
		uint column;
	};

	struct GridStyle {
		ValueTag length = 0px;

		struct GridlineStyle {
			float width = 0px;
			Color color = color_transparent;
		} gridline;
	};

	typedef GridStyle RowStyle, ColumnStyle;

public:
	struct Row {
		GridStyle style;
		std::vector<child_ptr> child_list;

		template<class... Ts>
		Row(GridStyle style, Ts... child_args) : style(style), child_list() {
			child_list.reserve(sizeof...(Ts)); (child_list.emplace_back(std::move(child_args)), ...);
		}
	};

public:
	template<class... Ts>
		requires (std::is_convertible_v<Ts, Row> && ...)
	TableLayout(std::vector<GridStyle> column_style, Ts... row_args) {
		table.reserve(sizeof...(Ts));
		row_list.reserve(sizeof...(Ts));
		column_list.assign(column_style.begin(), column_style.end());
		([&](Row row) {
			row.child_list.resize(column_list.size());
			table.emplace_back(std::vector<ChildInfo>(std::make_move_iterator(row.child_list.begin()), std::make_move_iterator(row.child_list.end())));
			row_list.emplace_back(row.style);
		}(std::move(row_args)), ...);
		UpdateGridlineSize();
		for (uint row_index = 0; row_index < table.size(); row_index++) {
			for (uint column_index = 0; column_index < table[row_index].size(); column_index++) {
				if (auto& item = table[row_index][column_index]; item.child != nullptr) {
					RegisterChild(item.child); SetChildIndex(item.child, { row_index, column_index });
				}
			}
		}
	}

	// child
protected:
	struct ChildInfo {
	public:
		child_ptr child;
		Size size;
	public:
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	};
	std::vector<std::vector<ChildInfo>> table;
protected:
	void SetChildIndex(WndObject& child, TablePosition pos) { WndObject::SetChildData<TablePosition>(child, pos); }
	TablePosition GetChildIndex(WndObject& child) const { return WndObject::GetChildData<TablePosition>(child); }
protected:
	child_ptr& GetChild(TablePosition pos) { return table[pos.row][pos.column].child; }

	// layout
protected:
	Size size_ref;
	Size size;
	Size gridline_size;
protected:
	struct GridInfo {
	public:
		GridStyle style;
		float length_ref = 0.0f;
		float offset = 0.0f;
		float length = 0.0f;
	public:
		GridInfo(GridStyle style) : style(style) {}
	public:
		float begin() const { return offset; }
		float end() const { return offset + length; }
		float gridline_width() const { return style.gridline.width; }
		float gridline_end() const { return end() + gridline_width(); }
	};
	std::vector<GridInfo> row_list;
	std::vector<GridInfo> column_list;
protected:
	Point GetGridOffset(TablePosition pos) const { return Point(column_list[pos.column].offset, row_list[pos.row].offset); }
	Size GetGridSize(TablePosition pos) const { return Size(column_list[pos.column].length, row_list[pos.row].length); }
	Rect GetGridRegion(TablePosition pos) const { return Rect(GetGridOffset(pos), GetGridSize(pos)); }
	Rect GetChildRegion(WndObject& child) const { return GetGridRegion(GetChildIndex(child)); }
protected:
	TablePosition HitTestGrid(Point point) {
		static auto cmp = [](const GridInfo& item, float offset) { return offset >= item.offset; };
		size_t row = std::lower_bound(row_list.begin(), row_list.end(), point.y, cmp) - row_list.begin() - 1;
		size_t column = std::lower_bound(column_list.begin(), column_list.end(), point.x, cmp) - column_list.begin() - 1;
		return { (uint)row, (uint)column };
	}
protected:
	void UpdateGridlineSize() {
		gridline_size.height = row_list.empty() ? 0.0f : -row_list.back().gridline_width();
		gridline_size.width = column_list.empty() ? 0.0f : -column_list.back().gridline_width();
		for (auto& row : row_list) { gridline_size.height += row.gridline_width(); }
		for (auto& column : column_list) { gridline_size.width += column.gridline_width(); }
	}
	void UpdateRowOffset(size_t index) {
		size.height = index == 0 ? 0.0f : row_list[index - 1].gridline_end();
		for (index; index < row_list.size(); index++) {
			row_list[index].offset = size.height;
			size.height += row_list[index].length + row_list[index].gridline_width();
		}
		if (!row_list.empty()) { size.height -= row_list.back().gridline_width(); }
	}
	void UpdateColumnOffset(size_t index) {
		size.width = index == 0 ? 0.0f : column_list[index - 1].gridline_end();
		for (index; index < column_list.size(); index++) {
			column_list[index].offset = size.width;
			size.width += column_list[index].length + column_list[index].gridline_width();
		}
		if (!column_list.empty()) { size.width -= column_list.back().gridline_width(); }
	}
protected:
	virtual Transform GetChildTransform(WndObject& child) const override {
		return GetChildRegion(child).point - point_zero;
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (this->size_ref != size_ref) {
			this->size_ref = size_ref;
			size_ref.width -= gridline_size.width; size_ref.height -= gridline_size.height;
			for (auto& column : column_list) {
				column.length_ref = ValueTag(column.style.length).ConvertToPixel(size_ref.width).value();
				column.length = 0;
			}
			for (uint row_index = 0; row_index < table.size(); row_index++) {
				row_list[row_index].length_ref = ValueTag(row_list[row_index].style.length).ConvertToPixel(size_ref.height).value();
				row_list[row_index].length = 0;
				for (uint column_index = 0; column_index < table[row_index].size(); column_index++) {
					if (auto& item = table[row_index][column_index]; item.child != nullptr) {
						item.size = UpdateChildSizeRef(item.child, Size(column_list[column_index].length_ref, row_list[row_index].length_ref));
						row_list[row_index].length = std::max(row_list[row_index].length, item.size.height);
						column_list[column_index].length = std::max(column_list[column_index].length, item.size.width);
					}
				}
			}
			UpdateRowOffset(0);
			UpdateColumnOffset(0);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
		TablePosition begin = HitTestGrid(draw_region.LeftTop()), end = HitTestGrid(Point(ceilf(draw_region.right()) - 1.0f, ceilf(draw_region.bottom()) - 1.0f));
		for (uint row_index = begin.row; row_index <= end.row; row_index++) {
			if (GridStyle::GridlineStyle gridline = row_list[row_index].style.gridline; gridline.width > 0.0f && gridline.color.IsVisible()) {
				figure_queue.add(Point(0.0f, row_list[row_index].end()), new Rectangle(Size(size.width, gridline.width), gridline.color));
			}
		}
		for (uint column_index = begin.column; column_index <= end.column; column_index++) {
			if (GridStyle::GridlineStyle gridline = column_list[column_index].style.gridline; gridline.width > 0.0f && gridline.color.IsVisible()) {
				figure_queue.add(Point(column_list[column_index].end(), 0.0f), new Rectangle(Size(gridline.width, size.height), gridline.color));
			}
		}
		for (uint row_index = begin.row; row_index <= end.row; row_index++) {
			for (uint column_index = begin.column; column_index <= end.column; column_index++) {
				if (auto& item = table[row_index][column_index]; item.child != nullptr) {
					DrawChild(item.child, GetGridRegion({ row_index, column_index }), figure_queue, draw_region);
				}
			}
		}
	}

	//message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (!Rect(point_zero, size).Contains(msg.point)) { return nullptr; }
		TablePosition pos = HitTestGrid(msg.point);
		Rect grid = GetGridRegion(pos); child_ptr& child = GetChild(pos);
		if (!grid.Contains(msg.point) || child == nullptr) { return this; }
		msg.point -= grid.point - point_zero; return child;
	}
};


END_NAMESPACE(WndDesign)