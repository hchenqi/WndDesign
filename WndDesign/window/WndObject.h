#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
protected:
	WndObject() {}

	virtual ~WndObject() {
		if (HasParent()) { GetParent().OnChildDetach(*this); parent = nullptr; }
	}


	// parent window
private:
	ref_ptr<WndObject> parent = nullptr;
private:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { assert(HasParent()); return *parent; }
	bool IsMyAncestor(const WndObject& wnd) const {
		for (ref_ptr<const WndObject> ancestor = this; ancestor != nullptr; ancestor = ancestor->parent) {
			if (&wnd == ancestor) { return true; }
		}
		return false;
	}


	// child window
private:
	void VerifyChild(const WndObject& child) const { 
		if (child.parent != this) { throw std::invalid_argument("invalid child window"); } 
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
	template<class T, class = std::enable_if_t<sizeof(T) <= sizeof(uint64)>>
	void SetChildData(WndObject& child, T data) const {
		VerifyChild(child); memcpy(&child.parent_specific_data, &data, sizeof(T));
	}
	template<class T, class = std::enable_if_t<sizeof(T) <= sizeof(uint64)>>
	T GetChildData(const WndObject& child) const {
		VerifyChild(child); T data; memcpy(&data, &child.parent_specific_data, sizeof(T)); return data;
	}


	// layout
protected:
	const Size SetChildSizeRef(WndObject& child, Size size_ref) {
		VerifyChild(child); return child.OnSizeRefChange(size_ref);
	}
	void SizeChanged(Size size) {
		if (HasParent()) { GetParent().OnChildSizeChange(*this, size); }
	}
private:
	virtual const Size OnSizeRefChange(Size size_ref) {}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) {}


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