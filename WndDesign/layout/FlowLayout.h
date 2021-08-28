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
		row_height(row_height), column_gap(column_gap), row_gap(row_gap), row_list({0}) {
	}

private:
	struct ChildInfo {
		child_ptr child;
		uint column_offset;
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
	ChildData GetChildData(const WndObject& child) {
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
		if (child_list.empty()) {
			assert(row_list.size() == 1);
			row_list.push_back(1);
			ChildInfo & info = child_list.emplace_back(std::move(child));
			info.column_offset = 0;
			info.width = UpdateChildSizeRef(info.child, Size(length_min, row_height)).width;
			SetChildData(info.child, { 0, 0 });
		} else {
			ChildInfo& info_back = child_list.back();
			auto [row_index, column_index] = GetChildData(info_back.child);
			uint column_offset = info_back.column_offset + info_back.width + column_gap;
			child_list.emplace_back(std::move(child));
			UpdateLayout(row_index, column_index + 1, column_gap);
		}
		SizeUpdated(size);
	}

private:
	Size size;
private:
	void UpdateLayout(row_index row_index, column_index column_index, uint column_offset) {
		for (child_index child_index = row_list[row_index] + column_index; child_index < child_list.size(); ++child_index) {
			ChildInfo& info = child_list[child_index];
			if (column_offset > 0 && column_offset + info.width > size.width) {
				row_list[row_index++] = child_index;
				column_offset = 0; column_index = 0;
			}
			info.column_offset = column_offset;
			SetChildData(info.child, { row_index, column_index });
			column_offset += info.width + column_gap; column_index++;
		}
#error unchecked
		row_list.erase(row_list.begin() + row_index, row_list.end());
		row_list.push_back(child_list.size());
		size.height = child_list.empty() ? 0 : row_index * (row_height + row_gap) + row_height;
	}

	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			UpdateLayout(0, 0, 0);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {
		auto [row_index, column_index] = GetChildData(child);
		child_index child_index = row_list[row_index] + column_index;
		ChildInfo& info = child_list[child_index];
		if (info.width != child_size.width) {
			info.width = child_size.width;
			UpdateLayout(row_index, column_index, info.column_offset);
			SizeUpdated(size);
		}
	}
};


END_NAMESPACE(WndDesign)