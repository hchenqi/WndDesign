#pragma once

#include "../window/WndObject.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class ListLayoutVertical : public WndObject {



private:
	struct ChildInfo {
		ref_ptr<WndObject> child;
		uint y;
		uint height;
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
	void AppendChild(WndObject& child) {
		AddChild(child);
		SetChildData(child, child_list.size());
		ChildInfo& info = child_list.emplace_back(&child, 0, 0);
		info.y = size.height;
		info.height = SetChildSizeRef(child, Size(size.width, length_min)).height;
		size.height += info.height;
		SizeChanged(size);
	}


private:
	Size size;


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