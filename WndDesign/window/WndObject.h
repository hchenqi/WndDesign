#pragma once

#include "../common/uncopyable.h"
#include "../style/cursor.h"
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
	virtual ~WndObject() { if (HasParent()) { GetParent().UnregisterChild(*this); } ReleaseFocus(); }

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
private:
	void VerifyChild(WndObject& child) const {
		if (child.parent != this) { throw std::invalid_argument("invalid child window"); }
	}
protected:
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
	Size UpdateChildSizeRef(WndObject& child, Size size_ref) { VerifyChild(child); child.OnSizeRefUpdate(size_ref); return child.GetSize(); }
	void SizeUpdated() { if (HasParent()) { GetParent().OnChildSizeUpdate(*this, GetSize()); } }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) {}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) {}
	virtual Size GetSize() { return size_empty; }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) { return this; }
	virtual Transform GetChildTransform(WndObject& child) const { return Transform::Identity(); }
public:
	Transform GetTransform(WndObject& descendent) const;
	Point ConvertPoint(WndObject& descendent, Point point) const;

	// paint
protected:
	void Redraw() { if (HasParent()) { GetParent().OnChildRedraw(*this, GetRedrawRegion()); } }
	void DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region);
	void DrawChild(WndObject& child, Rect child_region, FigureQueue& figure_queue, Rect draw_region);
protected:
	virtual Rect GetRedrawRegion() { return region_infinite; }
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
	void SetCapture() { SetChildCapture(*this); }
	void ReleaseCapture();
	void SetFocus();
	void ReleaseFocus();
private:
	void DispatchMouseMsg(MouseMsg msg);
protected:
	virtual void OnMouseMsg(MouseMsg msg) {}
	virtual void OnKeyMsg(KeyMsg msg) {}
	virtual void OnNotifyMsg(NotifyMsg msg) {}
};


inline Transform WndObject::GetTransform(WndObject& descendent) const {
	Transform transform = Transform::Identity();
	for (ref_ptr<WndObject> child = &descendent, parent = descendent.parent; child != this; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("ancestor and descentent have no relation"); }
		transform = transform * parent->GetChildTransform(*child);
	}
	return transform;
}

inline Point WndObject::ConvertPoint(WndObject& descendent, Point point) const {
	for (ref_ptr<WndObject> child = &descendent, parent = descendent.parent; child != this; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("ancestor and descentent have no relation"); }
		point *= parent->GetChildTransform(*child);
	}
	return point;
}

inline void WndObject::DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region) {
	VerifyChild(child); if (draw_region.IsEmpty()) { return; }
	Vector offset = child_offset - point_zero; draw_region -= offset;
	figure_queue.Offset(offset, [&]() { child.OnDraw(figure_queue, draw_region); });
}

inline void WndObject::DrawChild(WndObject& child, Rect child_region, FigureQueue& figure_queue, Rect draw_region) {
	VerifyChild(child); draw_region = draw_region.Intersect(child_region); if (draw_region.IsEmpty()) { return; }
	Vector offset = child_region.point - point_zero; draw_region -= offset;
	figure_queue.Group(offset, draw_region, [&]() { child.OnDraw(figure_queue, draw_region); });
}


END_NAMESPACE(WndDesign)