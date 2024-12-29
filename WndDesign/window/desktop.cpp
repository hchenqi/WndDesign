#include "desktop.h"
#include "../frame/DesktopFrame.h"
#include "../system/win32_api.h"
#include "../system/win32_ime.h"
#include "../system/cursor.h"
#include "../common/reversion_wrapper.h"


BEGIN_NAMESPACE(WndDesign)

Desktop& desktop = Desktop::Get();


Desktop::Desktop() {}

Desktop::~Desktop() {}

Desktop& Desktop::Get() {
	static Desktop desktop;
	return desktop;
}

DesktopFrame& Desktop::AddChild(std::unique_ptr<DesktopFrame> frame) {
	RegisterChild(*frame);
	frame->InitializeRegion(Win32::GetDesktopSize());
	frame->Show();
	return *frame_list.emplace_back(std::move(frame));
}

std::unique_ptr<DesktopFrame> Desktop::RemoveChild(DesktopFrame& frame) {
	auto it = std::find_if(frame_list.begin(), frame_list.end(), [&](const std::unique_ptr<DesktopFrame>& ptr) { return ptr.get() == &frame; });
	if (it == frame_list.end()) { throw std::invalid_argument("invalid desktop frame"); }
	frame.Hide();
	UnregisterChild(frame);
	std::unique_ptr<DesktopFrame> ptr = std::move(*it);	frame_list.erase(it);
	if (frame_list.empty()) { Terminate(); }
	return ptr;
}

DesktopFrame& Desktop::GetDesktopFrame(WndObject& wnd) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window not registered"); }
	}
	return static_cast<DesktopFrame&>(*child);
}

DesktopFrame& Desktop::GetDesktopFramePoint(WndObject& wnd, Point& point) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window not registered"); }
		point *= parent->GetChildTransform(*child);
	}
	return static_cast<DesktopFrame&>(*child);
}

void Desktop::RecreateFrameLayer() {
	for (auto& frame : frame_list) {
		frame->RecreateLayer();
	}
}

void Desktop::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	static_cast<DesktopFrame&>(child).Redraw(child_redraw_region);
}

void Desktop::SetTrack(WndObject& wnd) {
	if (!wnd_track_stack.empty() && wnd_track_stack.back() == &wnd) { return; }
	std::vector<ref_ptr<WndObject>> trace;
	ref_ptr<WndObject> curr = &wnd;
	while (!wnd_track_map.contains(curr)) {
		trace.push_back(curr);
		curr = curr->parent;
		if (curr == nullptr) { break; }
	}
	if (curr == nullptr) {
		LoseTrack();
	} else {
		wnd_track_stack.back()->OnNotifyMsg(NotifyMsg::MouseOut);
		for (size_t index = wnd_track_map[curr]; wnd_track_stack.size() > index; wnd_track_stack.pop_back()) {
			wnd_track_stack.back()->OnNotifyMsg(NotifyMsg::MouseLeave);
			wnd_track_map.erase(wnd_track_stack.back());
		}
	}
	for (; !trace.empty(); trace.pop_back()) {
		trace.back()->OnNotifyMsg(NotifyMsg::MouseEnter);
		wnd_track_stack.push_back(trace.back());
		wnd_track_map.emplace(trace.back(), wnd_track_stack.size());
	}
	wnd.OnNotifyMsg(NotifyMsg::MouseOver);
	SetCursor(wnd.cursor);
}

void Desktop::LoseTrack() {
	if (wnd_track_stack.empty()) { return; }
	wnd_track_stack.back()->OnNotifyMsg(NotifyMsg::MouseOut);
	for (auto wnd : reverse(wnd_track_stack)) {
		wnd->OnNotifyMsg(NotifyMsg::MouseLeave);
	}
	wnd_track_stack.clear();
	wnd_track_map.clear();
}

void Desktop::SetCapture(WndObject& wnd) {
	if (wnd_capture == &wnd) { return; }
	DesktopFrame& frame = GetDesktopFrame(wnd);
	if (frame_capture != &frame) { Win32::SetCapture(frame.hwnd); }
	if (wnd_capture != &wnd) { LoseCapture(); }
	frame_capture = &frame; wnd_capture = &wnd;
}

void Desktop::ReleaseCapture(WndObject& wnd) {
	if (wnd_capture == &wnd) {
		Win32::ReleaseCapture();
	}
}

void Desktop::LoseCapture() {
	frame_capture = nullptr; wnd_capture = nullptr;
}

void Desktop::DispatchMouseMsg(DesktopFrame& frame, MouseMsg msg) {
	if (wnd_capture != nullptr) {
		msg.point *= frame.GetDescendentTransform(*wnd_capture).Invert();
		wnd_capture->OnMouseMsg(msg);
	} else {
		for (ref_ptr<WndObject> curr = &frame;;) {
			ref_ptr<WndObject> next = curr->HitTest(msg);
			if (next == nullptr) {
				return;
			} else if (next == curr) {
				SetTrack(*curr);
				curr->OnMouseMsg(msg);
				return;
			} else {
				curr = next;
			}
		}
	}
}

void Desktop::SetFocus(WndObject& wnd) {
	if (wnd_focus == &wnd) { return; }
	DesktopFrame& frame = GetDesktopFrame(wnd);
	if (frame_focus != &frame) { Win32::SetFocus(frame.hwnd); }
	LoseFocus();
	frame_focus = &frame; wnd_focus = &wnd;
	ime_enabled_wnd.contains(&wnd) ? WndDesign::ImeEnable(frame.hwnd) : WndDesign::ImeDisable(frame.hwnd);
}

void Desktop::ReleaseFocus(WndObject& wnd) {
	if (wnd_focus == &wnd) {
		Win32::SetFocus(nullptr);
	}
}

void Desktop::LoseFocus() {
	if (wnd_focus != nullptr) {
		wnd_focus->OnNotifyMsg(NotifyMsg::LoseFocus);
		frame_focus = nullptr; wnd_focus = nullptr;
	}
}

void Desktop::DispatchKeyMsg(KeyMsg msg) {
	if (wnd_focus != nullptr) {
		wnd_focus->OnKeyMsg(msg);
	}
}

void Desktop::ImeSetPosition(WndObject& wnd, Point point) {
	DesktopFrame& frame = GetDesktopFramePoint(wnd, point);
	WndDesign::ImeSetPosition(frame.hwnd, point);
}

void Desktop::ReleaseWindow(WndObject& wnd) {
	if (&wnd == &desktop) { return; }
	if (wnd_track_map.contains(&wnd)) {
		wnd_track_stack.clear();
		wnd_track_map.clear();
	}
	ReleaseCapture(wnd);
	ReleaseFocus(wnd);
	ImeDisable(wnd);
}

void Desktop::MessageLoop() { Win32::MessageLoop(); }

void Desktop::Terminate() { frame_list.clear(); Win32::Terminate(); }


END_NAMESPACE(WndDesign)