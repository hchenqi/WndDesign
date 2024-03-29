#pragma once

#include "../common/uncopyable.h"
#include "../style/cursor_style.h"
#include "../geometry/geometry.h"
#include "../figure/figure_queue.h"
#include "../message/message.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {
private:
	friend class Desktop;

protected:
	WndObject() {}
public:
	virtual ~WndObject();

	// style
protected:
	Cursor cursor = Cursor::Default;

	// parent
private:
	ref_ptr<WndObject> parent = nullptr;
protected:
	bool HasParent() const { return parent != nullptr; }
	WndObject& GetParent() const { if (!HasParent()) { throw std::invalid_argument("window has no parent"); } return *parent; }

	// child
protected:
	void VerifyChild(WndObject& child) const {
		if (child.parent != this) { throw std::invalid_argument("invalid child window"); }
	}
	void RegisterChild(WndObject& child) {
		if (child.HasParent()) { throw std::invalid_argument("window already has a parent"); } child.parent = this;
	}
	void UnregisterChild(WndObject& child) {
		VerifyChild(child);
		if (child_track == &child) { SetChildTrack(*this); }
		if (child_capture == &child) { ReleaseCapture(); }
		child.parent = nullptr;
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
protected:
	virtual Transform GetChildTransform(WndObject& child) const { return Transform::Identity(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) { return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) {}
public:
	Transform GetDescendentTransform(WndObject& descendent) const;
	Point ConvertDescendentPoint(WndObject& descendent, Point point) const;

	// paint
protected:
	void Redraw(Rect redraw_region) { if (HasParent() && !redraw_region.IsEmpty()) { GetParent().OnChildRedraw(*this, redraw_region); } }
	void DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region);
	void DrawChild(WndObject& child, Rect child_region, FigureQueue& figure_queue, Rect draw_region);
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) {}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) {}

	// message
private:
	ref_ptr<WndObject> child_track = nullptr;
	ref_ptr<WndObject> child_capture = nullptr;
private:
	void SetChildTrack(WndObject& child);
	void LoseTrack();
	void SetChildCapture(WndObject& child);
	void LoseCapture();
protected:
	void SetCapture();
	void ReleaseCapture();
	void SetFocus();
	void ReleaseFocus();
private:
	void DispatchMouseMsg(MouseMsg msg);
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) { return this; }
protected:
	virtual void OnMouseMsg(MouseMsg msg) {}
	virtual void OnKeyMsg(KeyMsg msg) {}
	virtual void OnNotifyMsg(NotifyMsg msg) {}
};


END_NAMESPACE(WndDesign)