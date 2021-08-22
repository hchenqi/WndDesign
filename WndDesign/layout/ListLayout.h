#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ListLayout;


template<>
class ListLayout<Vertical> : public WndType<Relative, Auto> {
public:
	using child_ptr = child_ptr<Relative, Auto>;
public:
	ListLayout() {}

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
		info.height = SetChildSizeRef(info.child, Size(size.width, length_min)).height;
		size.height += info.height;
		SizeChanged(size);
	}

private:
	Size size;
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			size.height = 0;
			for (auto& info : child_list) {
				info.y = size.height;
				info.height = SetChildSizeRef(*info.child, Size(size.width, length_min)).height;
				size.height += info.height;
			}
		}
		return size;
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
		uint64 index = GetChildData(child); assert(index < child_list.size());
		if (child_list[index].height != child_size.height) {
			child_list[index].height = child_size.height;
			size.height = child_list[index].y;
			for (; index < child_list.size(); index++) {
				child_list[index].y = size.height;
				size.height += child_list[index].height;
			}
			SizeChanged(size);
		}
	}
};


END_NAMESPACE(WndDesign)