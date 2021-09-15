#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"
#include "../figure/figure_queue.h"
#include "../message/message.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
protected:
	virtual ~WndObject() {}

	// parent window
private:
	ref_ptr<WndObject> parent = nullptr;
private:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { assert(HasParent()); return *parent; }

	// child window
private:
	void VerifyChild(const WndObject& child) const {
		if (child.parent != this) { throw std::invalid_argument("invalid child window"); } 
	}
protected:
	void RegisterChild(WndObject& child) {
		if (child.HasParent()) { throw std::invalid_argument("window already has a parent"); }
		child.parent = this;
	}
	void UnregisterChild(WndObject& child) {
		VerifyChild(child); child.parent = nullptr;
	}

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
	const Size UpdateChildSizeRef(WndObject& child, Size size_ref) const { VerifyChild(child); return child.OnSizeRefUpdate(size_ref); }
	void SizeUpdated(Size size) const { if (HasParent()) { GetParent().OnChildSizeUpdate(*this, size); } }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) { return size_ref; }
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) {}

	// paint
protected:
	void Redraw(Rect redraw_region) const { if (HasParent()) { GetParent().OnChildRedraw(*this, redraw_region); } }
	void DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region) const {
		VerifyChild(child); Vector offset = child_offset - point_zero;
		figure_queue.PushOffset(offset); child.OnDraw(figure_queue, draw_region + offset); figure_queue.PopOffset(offset);
	}
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) {}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const {}

	// message
private:
	bool is_mouse_tracked = false;
	bool is_mouse_captured = false;
	bool is_on_focus = false;
protected:
	void SetCapture() const {}
	void ReleaseCapture() const {}
	void SetFocus() const {}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) { return this; }
private:
	virtual bool OnMouseMsg(MouseMsg msg) { return false; }
	virtual bool OnKeyMsg(KeyMsg msg) { return false; }
	virtual void OnNotifyMsg(NotifyMsg msg) {}
};


END_NAMESPACE(WndDesign)