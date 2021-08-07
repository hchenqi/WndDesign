#pragma once

#include "../frame/OverlapWndFrame.h"

#include <list>


BEGIN_NAMESPACE(WndDesign)


class OverlapLayout : public WndObject {


	// child window
private:
	struct ChildInfo {
		OverlapWndFrame& wnd;
		Rect region;
		std::list<ChildInfo>::iterator index;
	};

	std::list<ChildInfo> child_list;  // (begin)topmost ---> bottommost(end)

private:
	void SetChildData(WndObject& child, ChildInfo& child_container) {
		WndObject::SetChildData<ChildInfo*>(child, &child_container);
	}
	ChildInfo& GetChildData(WndObject& child) {
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
	const Size GetSize() const { return size; }
	void SetSize(Size size) { this->size = size; }

private:
	virtual const Size UpdateLayout(Size size) override {
		if (size != GetSize()) {
			SetSize(size);
			for (auto& child_info : child_list) {
				Rect region = child_info.wnd.UpdateLayoutRegion(size);
				if (child_info.region != region) {
					InvalidateRegion(child_info.region);
					InvalidateRegion(region);
					child_info.region = region;
				}
			}
		}
		return size;
	}

	virtual void UpdateChildLayout(WndObject& child) override {
		ChildInfo& info = GetChildData(child);
		Rect region = info.wnd.UpdateLayoutRegion(size);
		if (info.region != region) {
			InvalidateRegion(info.region);
			InvalidateRegion(region);
			info.region = region;
		}

	}

	virtual void InvalidateChildSize(WndObject& child) override {
		ChildInfo& info = GetChildData(child);
		info.region = 
	}


};


END_NAMESPACE(WndDesign)