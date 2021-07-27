#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
protected:
	WndObject() {}
	virtual ~WndObject() {
		if (layout_invalid) { GetReflowQueue().RemoveWnd(*this); }
		if (region_invalid) { GetRedrawQueue().RemoveWnd(*this); }
	}


	// parent window
private:
	ref_ptr<WndObject> parent = nullptr;
private:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { assert(HasParent()); return *parent; }
	bool IsMyAncestor(WndObject& wnd) const {
		for (ref_ptr<WndObject> ancestor = parent; ancestor != nullptr; ancestor = ancestor->parent) {
			if (&wnd == ancestor) { return true; }
		}
		return false;
	}

	// child window
private:
	void VerifyChild(WndObject& child) const { if (child.parent != this) { throw std::invalid_argument("window is not a child"); } }
protected:
	void AddChild(WndObject& child) {
		if (child.HasParent()) { throw std::invalid_argument("window already has a parent"); }
		if (IsMyAncestor(child)) { throw std::invalid_argument("window is an ancestor"); }
		child.parent = this;
	}
public:
	void RemoveChild(WndObject& child) { VerifyChild(child); OnChildDetach(child); child.parent = nullptr; }
private:
	virtual void OnChildDetach(WndObject& child) {}

	// data used by parent window
private:
	uint64 parent_specific_data = 0;
protected:
	template<class T> void SetChildData(WndObject& child, T data) const {
		static_assert(sizeof(T) <= sizeof(uint64)); VerifyChild(child);
		memcpy(&child.parent_specific_data, &data, sizeof(T));
	}
	template<class T> T GetChildData(WndObject& child) const {
		static_assert(sizeof(T) <= sizeof(uint64)); VerifyChild(child);
		T data; memcpy(&data, &child.parent_specific_data, sizeof(T)); return data;
	}


	// layout
private:
	bool layout_invalid;
protected:
	void InvalidateLayout() { if (!layout_invalid) { layout_invalid = true; GetReflowQueue().AddWnd(*this); } }
	void InvalidateSize() { if (HasParent()) { GetParent().InvalidateChildSize(*this); } }
 private:
	virtual void InvalidateChildSize(WndObject& child) {}
	virtual const Size UpdateLayout(Size size) { return size; }



	// painting
private:
	bool region_invalid = false;
	Rect invalid_region = region_empty;


public:
	bool HasInvalidRegion() const { return invalid_region != region_empty; }
	const Rect GetInvalidRegion() const { return invalid_region; }

	void InvalidateChildRegion(WndObject& child) {
		Rect child_region = GetChildRegion(child);
		InvalidateRegion(child_region.Intersect(child.GetInvalidRegion() + (child_region.point - point_zero)));
	}

	virtual const Rect GetChildRegion(WndObject& child) { return region_empty; }

	void InvalidateRegion(Rect invalid_region) {
		this->invalid_region = this->invalid_region.Union(invalid_region);
		if (!region_invalid) {
			region_invalid = true;
			GetRedrawQueue().AddWnd(*this);
		}
	}

	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region) {}


	// message
private:
	virtual void Handler(Msg msg, Para para) {}
};


END_NAMESPACE(WndDesign)