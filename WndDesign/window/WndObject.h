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
	void UpdateChildSizeRef(WndObject& child, Size size_ref) { VerifyChild(child); child.OnSizeRefUpdate(size_ref); }
	void SizeUpdated() { if (HasParent()) { GetParent().OnChildSizeUpdate(*this); } }
	Size GetChildSize(WndObject& child) { VerifyChild(child); return child.GetSize(); }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) {}
	virtual void OnChildSizeUpdate(WndObject& child) {}
	virtual Size GetSize() { return size_empty; }
protected:
	virtual Vector GetChildOffset(WndObject& child) { return vector_zero; }
	virtual ref_ptr<WndObject> HitTest(Point& point) { return this; }

	// paint
protected:
	void Redraw() { if (HasParent()) { GetParent().OnChildRedraw(*this); } }
	Rect GetChildRedrawRegion(WndObject& child) { VerifyChild(child); return child.GetRedrawRegion(); }
protected:
	void DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region) {
		VerifyChild(child); if (draw_region.IsEmpty()) { return; }
		Vector offset = child_offset - point_zero; draw_region -= offset;
		figure_queue.Offset(offset, [&]() { child.OnDraw(figure_queue, draw_region); });
	}
	void DrawChild(WndObject& child, Rect child_region, FigureQueue& figure_queue, Rect draw_region) {
		VerifyChild(child); draw_region = draw_region.Intersect(child_region); if (draw_region.IsEmpty()) { return; }
		Vector offset = child_region.point - point_zero; draw_region -= offset;
		figure_queue.Group(offset, draw_region, [&]() { child.OnDraw(figure_queue, draw_region); });
	}
protected:
	virtual void OnChildRedraw(WndObject& child) {}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) {}
	virtual Rect GetRedrawRegion() { return region_infinite; }

	// message
private:
	ref_ptr<WndObject> child_track = nullptr;
	ref_ptr<WndObject> child_capture = nullptr;
	ref_ptr<WndObject> child_focus = nullptr;
protected:
	void SetCapture();
	void ReleaseCapture();
	void SetFocus();
protected:
	virtual void OnMouseMsg(MouseMsg msg) {}
	virtual void OnKeyMsg(KeyMsg msg) {}
	virtual void OnNotifyMsg(NotifyMsg msg) {}
};


END_NAMESPACE(WndDesign)