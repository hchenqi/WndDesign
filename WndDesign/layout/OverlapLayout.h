#pragma once

#include "../frame/OverlapWndFrame.h"

#include <list>


BEGIN_NAMESPACE(WndDesign)


class OverlapLayout : public WndObject {


	// child window
private:
	struct ChildInfo {
		OverlapWndFrame& child;
		Rect region;
		std::list<ChildInfo>::iterator index;
	};

	std::list<ChildInfo> child_list;  // (begin)topmost ---> bottommost(end)

private:
	void SetChildData(WndObject& child, ChildInfo& info) {
		WndObject::SetChildData<ChildInfo*>(child, &info);
	}
	ChildInfo& GetChildData(const WndObject& child) {
		return *WndObject::GetChildData<ChildInfo*>(child);
	}

public:
	void AddChild(OverlapWndFrame& child) {
		WndObject::AddChild(child);
		ChildInfo& info = child_list.emplace_front(child, region_empty); info.index = child_list.begin();
		SetChildData(child, info);
	}

private:
	virtual void OnChildDetach(WndObject& child) override {
		ChildInfo& info = GetChildData(child);
		InvalidateRegion(info.region);
		child_list.erase(info.index);
	}


	// layout
private:
	Size size;

private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			size = size_ref;
			for (auto& info : child_list) {
				info.region.size = info.child.OnSizeRefUpdate(size);
				info.region.point = info.child.GetPosition();
			}
		}
		return size;
	}

	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {
		ChildInfo& info = GetChildData(child);
		info.region.size = child_size;
		info.region.point = info.child.GetPosition();
	}
};


END_NAMESPACE(WndDesign)