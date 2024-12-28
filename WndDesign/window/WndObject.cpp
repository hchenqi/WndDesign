#include "WndObject.h"
#include "Desktop.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


WndObject::~WndObject() {
	if (HasParent()) {
		GetParent().UnregisterChild(*this);
	}
	ReleaseFocus();
}

WndObject& WndObject::GetDirectChild(WndObject& descendent) const {
	ref_ptr<WndObject> child = &descendent;
	while (child->parent != this) {
		if (child->parent == nullptr) { throw std::invalid_argument("invalid descendent window"); }
		child = child->parent;
	}
	return *child;
}

Transform WndObject::GetDescendentTransform(WndObject& descendent) const {
	Transform transform = Transform::Identity();
	for (ref_ptr<WndObject> child = &descendent, parent = descendent.parent; child != this; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("invalid descendent window"); }
		transform = transform * parent->GetChildTransform(*child);
	}
	return transform;
}

Point WndObject::ConvertDescendentPoint(WndObject& descendent, Point point) const {
	for (ref_ptr<WndObject> child = &descendent, parent = descendent.parent; child != this; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("invalid descendent window"); }
		point *= parent->GetChildTransform(*child);
	}
	return point;
}

void WndObject::DrawChild(WndObject& child, Point child_offset, FigureQueue& figure_queue, Rect draw_region) {
	VerifyChild(child);
	if (draw_region.IsEmpty()) { return; }
	Vector offset = child_offset - point_zero; draw_region -= offset;
	figure_queue.Offset(offset, [&]() { child.OnDraw(figure_queue, draw_region); });
}

void WndObject::DrawChild(WndObject& child, Rect child_region, FigureQueue& figure_queue, Rect draw_region) {
	VerifyChild(child);
	draw_region = draw_region.Intersect(child_region); if (draw_region.IsEmpty()) { return; }
	Vector offset = child_region.point - point_zero; draw_region -= offset;
	figure_queue.Group(offset, draw_region, [&]() { child.OnDraw(figure_queue, draw_region); });
}

void WndObject::SetChildTrack(WndObject& child) {
	if (child_track == &child) { return; }
	if (child_track == nullptr) {
		OnNotifyMsg(NotifyMsg::MouseEnter);
	} else if (child_track != this) {
		child_track->LoseTrack();
	}
	child_track = &child;
	if (child_track == this) {
		SetCursor(cursor);
		OnNotifyMsg(NotifyMsg::MouseHover);
	}
}

void WndObject::LoseTrack() {
	if (child_track != nullptr) {
		if (child_track != this) {
			child_track->LoseTrack();
		}
		child_track = nullptr;
		OnNotifyMsg(NotifyMsg::MouseLeave);
	}
}

void WndObject::SetChildCapture(WndObject& child) {
	if (child_capture == &child) { return; }
	LoseCapture();
	child_capture = &child;
	if (parent == &desktop) { return desktop.SetCapture(*this); }
	if (HasParent()) { GetParent().SetChildCapture(*this); }
}

void WndObject::LoseCapture() {
	if (child_capture != nullptr) {
		if (child_capture != this) {
			child_capture->LoseCapture();
		}
		child_capture = nullptr;
	}
}

void WndObject::SetCapture() { SetChildCapture(*this); }

void WndObject::ReleaseCapture() { desktop.ReleaseCapture(); }

void WndObject::SetFocus() { desktop.SetFocus(*this); }

void WndObject::ReleaseFocus() { desktop.ReleaseFocus(*this); }

void WndObject::DispatchMouseMsg(MouseMsg msg) {
	if (child_capture == this) {
		return OnMouseMsg(msg);
	}
	if (child_capture != nullptr) {
		msg.point *= GetChildTransform(*child_capture).Invert();
		return child_capture->DispatchMouseMsg(msg);
	}
	ref_ptr<WndObject> child = HitTest(msg);
	if (child == nullptr) {
		LoseTrack();
		SetCursor(cursor);
		return;
	}
	if (child != this) {
		VerifyChild(*child);
	}
	SetChildTrack(*child);
	return child == this ? OnMouseMsg(msg) : child->DispatchMouseMsg(msg);
}


END_NAMESPACE(WndDesign)