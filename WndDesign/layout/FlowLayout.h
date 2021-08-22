#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class FlowLayout;


template<>
class FlowLayout<Vertical> : public WndType<Relative, Auto> {
public:
	using child_ptr = child_ptr<Auto, Relative>;
public:
	FlowLayout(uint row_height, uint column_gap, uint row_gap) :
		row_height(row_height), column_gap(column_gap), row_gap(row_gap) {
	}

private:
	struct ChildInfo {
		child_ptr child;
		uint row_offset;
		uint width;
	};
	std::vector<ChildInfo> child_list;

	using child_index = uint;
	std::vector<child_index> row_list;

private:
	using row_index = uint;
	using column_index = uint;
	using ChildData = std::pair<row_index, column_index>;

	void SetChildData(WndObject& child, ChildData data) {
		WndObject::SetChildData<ChildData>(child, data);
	}
	ChildData GetChildData(WndObject& child) {
		return WndObject::GetChildData<ChildData>(child);
	}

private:
	uint row_height;
	uint column_gap;
	uint row_gap;
private:
	uint GetRowPosition(uint row_index) const { return row_index * (row_height + row_gap); }

public:
	void AppendChild(child_ptr child) {
		RegisterChild(child);
		SetChildData(child, child_list.size());
		ChildInfo& info = child_list.emplace_back(std::move(child));

		info.y = size.height;
		info.height = SetChildSizeRef(info.child, Size(size.width, length_min)).height;
		size.height += info.height;

		SizeChanged(size);
	}

private:
	Size size;
private:
	void UpdateLayout(row_index row_index, column_index column_index, uint row_offset, child_index child_index) {

	}

	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			uint row_index = 0, column_index = 0;
			uint row_offset = 0;
			row_list.clear();
			for (child_index child_index = 0; child_index < child_list.size(); ++child_index) {
				ChildInfo& info = child_list[child_index];
				if (row_offset > 0 && row_offset + info.width > size.width) {
					row_list.push_back(child_index); row_index++;
					row_offset = 0; column_index = 0;
				}
				info.row_offset = row_offset;
				SetChildData(info.child, { row_index, column_index });
				row_offset += info.width + column_gap; column_index++;
			}
			row_list.push_back(child_list.size());
			size.height = child_list.empty() ? 0 : row_index * (row_height + row_gap) + row_height;
		}
		return size;
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) override {
		auto [row_index, column_index] = GetChildData(child);
		child_index child_index = row_list[row_index] + column_index;
		if (child_list[child_index].width != child_size.width) {
			child_list[child_index].width = child_size.width;


		}
	}
};


END_NAMESPACE(WndDesign)