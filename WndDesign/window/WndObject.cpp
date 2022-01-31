#include "WndObject.h"
#include "Desktop.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


void WndObject::SetChildTrack(WndObject& child) {
	if (child_track == &child) { return; }
	if (child_track == nullptr) {
		OnNotifyMsg(NotifyMsg::MouseEnter);
	} else if (child_track != this) {
		child_track->LoseTrack();
	}
	child_track = &child;
	if (child_track == this) { SetCursor(cursor); OnNotifyMsg(NotifyMsg::MouseHover); }
}

void WndObject::LoseTrack() {
	if (child_track != nullptr) {
		if (child_track != this) { child_track->LoseTrack(); }
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
		if (child_capture != this) { child_capture->LoseCapture(); }
		child_capture = nullptr;
	}
}

void WndObject::ReleaseCapture() { desktop.ReleaseCapture(); }

void WndObject::SetFocus() { desktop.SetFocus(*this); }

void WndObject::ReleaseFocus() { desktop.ReleaseFocus(*this); }

void WndObject::DispatchMouseMsg(MouseMsg msg) {
	if (child_capture == this) { return OnMouseMsg(msg); }
	if (child_capture != nullptr) { msg.point *= GetChildTransform(*child_capture).Invert(); return child_capture->DispatchMouseMsg(msg); }
	ref_ptr<WndObject> child = HitTest(msg.point);
	if (child == nullptr) { LoseTrack(); SetCursor(cursor); return; }
	SetChildTrack(*child);
	if (child == this) { return OnMouseMsg(msg); }
	return child->DispatchMouseMsg(msg);
}


END_NAMESPACE(WndDesign)