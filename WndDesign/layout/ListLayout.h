#pragma once

#include "../window/wnd_traits.h"

#include <array>
#include <vector>


BEGIN_NAMESPACE(WndDesign)


constexpr uint dynamic_extent = -1;


template<class Direction, uint size = dynamic_extent>
class ListLayout;


template<class Direction, uint size = dynamic_extent>
class _ListLayout_Base {

	std::array<>
};


template<class Direction>
class _ListLayout_Base<Direction, dynamic_extent> {

};



template<>
class ListLayout<Vertical> : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;
public:
	ListLayout(uint row_gap) : row_gap(row_gap) {}

private:
	struct ChildInfo {
		child_ptr child;
		uint y = 0;
		uint height = 0;
	};
	std::vector<ChildInfo> child_list;

private:
	void SetChildData(WndObject& child, uint64 index) {
		WndObject::SetChildData<uint64>(child, index);
	}
	uint64 GetChildData(WndObject& child) {
		return WndObject::GetChildData<uint64>(child);
	}

public:
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
		uint64 index = GetChildData(child); assert(index < child_list.size());
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
};


END_NAMESPACE(WndDesign)