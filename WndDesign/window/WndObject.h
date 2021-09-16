#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"
#include "../figure/figure_queue.h"
#include "../message/message.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
private:
	friend class Desktop;

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
private:
	virtual const Vector GetChildOffset(const WndObject& child) { return vector_zero; }
	virtual ref_ptr<WndObject> HitTest(Point& point) { return this; }

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
	void SetCapture();
	void ReleaseCapture();
	void SetFocus();
protected:
	void PassMouseMsg(MouseMsg msg);
	void PassKeyMsg(KeyMsg msg);
protected:
	virtual void OnMouseMsg(MouseMsg msg) { if (msg.wheel_delta != 0) { PassMouseMsg(msg); } }
	virtual void OnKeyMsg(KeyMsg msg) { PassKeyMsg(msg); }
	virtual void OnNotifyMsg(NotifyMsg msg) {}
};


END_NAMESPACE(WndDesign)