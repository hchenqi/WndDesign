#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
protected:
	WndObject() {}
	virtual ~WndObject() {}

	// parent window
private:
	ref_ptr<WndObject> parent = nullptr;
private:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { assert(HasParent()); return *parent; }
	bool IsMyAncestor(const WndObject& wnd) const {
		for (ref_ptr<const WndObject> ancestor = parent; ancestor != nullptr; ancestor = ancestor->parent) {
			if (&wnd == ancestor) { return true; }
		}
		return false;
	}

	// child window
private:
	void VerifyChild(const WndObject& child) const { 
		if (child.parent != this) { throw std::invalid_argument("window is not a child"); } 
	}
protected:
	void AddChild(WndObject& child) {
		if (child.HasParent()) { throw std::invalid_argument("window already has a parent"); }
		if (IsMyAncestor(child)) { throw std::invalid_argument("window is an ancestor"); }
		child.parent = this;
	}
public:
	void RemoveChild(WndObject& child) { 
		VerifyChild(child); OnChildDetach(child); child.parent = nullptr; 
	}
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
	template<class T> T GetChildData(const WndObject& child) const {
		static_assert(sizeof(T) <= sizeof(uint64)); VerifyChild(child);
		T data; memcpy(&data, &child.parent_specific_data, sizeof(T)); return data;
	}

	// layout
protected:
	void InvalidateSize() { if (HasParent()) { GetParent().InvalidateChildSize(*this); } }
protected:
	void UpdateChildLayout(WndObject& child, Size size) const {
		VerifyChild(child); child.UpdateLayout(size);
	}
private:
	virtual void InvalidateChildSize(const WndObject& child) {}
	virtual const Size UpdateLayout(Size size) { return size; }

	// paint
protected:
	void InvalidateRegion(Rect invalid_region) { if (HasParent()) { GetParent().InvalidateChildRegion(*this, invalid_region); } }
protected:
	void PaintChildRegion(WndObject& child, FigureQueue& figure_queue, Rect invalid_region) const {
		VerifyChild(child); child.OnPaint(figure_queue, invalid_region); 
	}
private:
	virtual void InvalidateChildRegion(const WndObject& child, Rect child_invalid_region) {}
	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region) const {}

	// message
private:
	virtual void Handler(Msg msg, Para para) {}
};


END_NAMESPACE(WndDesign)