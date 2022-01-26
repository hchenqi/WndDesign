#include "WndObject.h"
#include "Desktop.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


WndObject::~WndObject() {
	if (!HasParent()) { return; }
	if (child_track == this) { GetParent().SetChildTrack(GetParent()); }
	if (child_capture == this) { ReleaseCapture(); }
	if (child_focus == this) { ReleaseFocus(); }
}

void WndObject::SetChildTrack(WndObject& child) {
	if (child_track == &child) { return; }
	if (child_track == nullptr) {
		OnNotifyMsg(NotifyMsg::MouseEnter);
	} else if (child_track != this) {
		child_track->LoseTrack();
	}
	child_track = &child;
	if (child_track == this) { OnNotifyMsg(NotifyMsg::MouseHover); }
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

void WndObject::SetChildFocus(WndObject& child) {
	child_focus = &child;
	if (parent == &desktop) { return desktop.SetFocus(*this, child); }
	if (HasParent()) { GetParent().SetChildFocus(child); }
}

void WndObject::ReleaseChildFocus(WndObject& child) {
	if (child_focus != &child) { return; }
	child_focus = nullptr;
	if (parent == &desktop) { return desktop.ReleaseFocus(child); }
	if (HasParent()) { GetParent().ReleaseChildFocus(child); }
}

void WndObject::ReleaseCapture() { desktop.ReleaseCapture(); }

void WndObject::DispatchMouseMsg(MouseMsg msg) {
	if (child_capture == this) { return OnMouseMsg(msg); }
	if (child_capture != nullptr) { msg.point -= GetChildOffset(*child_capture); return child_capture->DispatchMouseMsg(msg); }
	ref_ptr<WndObject> child = HitTest(msg.point);
	if (child == nullptr) { LoseTrack(); return SetCursor(CursorStyle::Default); }
	SetChildTrack(*child);
	if (child == this) { return OnMouseMsg(msg); }
	return child->DispatchMouseMsg(msg);
}


END_NAMESPACE(WndDesign)