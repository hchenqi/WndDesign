#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"
#include "../figure/figure_queue.h"
#include "../message/message.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
private:
	friend class Desktop;
protected:
	WndObject() {}
public:
	virtual ~WndObject();

	// parent
private:
	ref_ptr<WndObject> parent = nullptr;
protected:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { if (!HasParent()) { throw std::invalid_argument("window has no parent"); } return *parent; }

	// child
private:
	void VerifyChild(WndObject& child) const {
		if (child.parent != this) { throw std::invalid_argument("invalid child window"); }
	}
protected:
	void RegisterChild(WndObject& child) {
		if (child.HasParent()) { throw std::invalid_argument("window already has a parent"); } child.parent = this;
	}
	void UnregisterChild(WndObject& child) {
		VerifyChild(child); child.parent = nullptr;
	}

	// parent data
private:
	uint64 parent_data = 0;
protected:
	template<class T, class = std::enable_if_t<sizeof(T) <= sizeof(uint64)>>
	void SetChildData(WndObject& child, T data) {
		VerifyChild(child); memcpy(&child.parent_data, &data, sizeof(T));
	}
	template<class T, class = std::enable_if_t<sizeof(T) <= sizeof(uint64)>>
	T GetChildData(WndObject& child) const {
		VerifyChild(child); T data; memcpy(&data, &child.parent_data, sizeof(T)); return data;
	}

	// layout
protected:
	Size UpdateChildSizeRef(WndObject& child, Size size_ref) { VerifyChild(child); return child.OnSizeRefUpdate(size_ref); }
	void SizeUpdated(Size size) { if (HasParent()) { GetParent().OnChildSizeUpdate(*this, size); } }
private:
	virtual Size OnSizeRefUpdate(Size size_ref) { return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) {}
private:
	virtual Vector GetChildOffset(WndObject& child) { return vector_zero; }
	virtual ref_ptr<WndObject> HitTest(Point& point) { return this; }

	// paint
protected:
	void Redraw(Rect redraw_region) { if (HasParent()) { GetParent().OnChildRedraw(*this, redraw_region); } }
	void DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region) {
		if (draw_region.IsEmpty()) { return; } VerifyChild(child); Vector offset = child_offset - point_zero;
		figure_queue.PushOffset(offset); child.OnDraw(figure_queue, draw_region - offset); figure_queue.PopOffset(offset);
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) {}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) {}

	// message
private:
	bool is_mouse_tracked = false;
	bool is_mouse_captured = false;
	bool is_on_focus = false;
	bool ime_aware = false;
protected:
	void SetCapture();
	void ReleaseCapture();
	void SetFocus();
protected:
	void PassMouseMsg(MouseMsg msg) { if (HasParent()) { msg.point += GetParent().GetChildOffset(*this); GetParent().OnMouseMsg(msg); } }
	void PassKeyMsg(KeyMsg msg) { if (HasParent()) { GetParent().OnKeyMsg(msg); } }
protected:
	virtual void OnMouseMsg(MouseMsg msg) { return PassMouseMsg(msg); }
	virtual void OnKeyMsg(KeyMsg msg) { return PassKeyMsg(msg); }
	virtual void OnNotifyMsg(NotifyMsg msg) { if (msg == NotifyMsg::MouseHover) { SetCursor(Cursor::Default); } }
};


END_NAMESPACE(WndDesign)