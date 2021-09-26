#include "desktop.h"
#include "../system/win32_api.h"
#include "../system/win32_ime.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)

Desktop desktop;


void Desktop::AddChild(frame_ptr frame) {
	RegisterChild(*frame);
	frame_list.push_back(std::move(frame));
	frame_list.back()->Show();
}

void Desktop::RemoveChild(frame_ref frame) {
	auto it = std::find_if(frame_list.begin(), frame_list.end(), [&](const frame_ptr& ptr) { return ptr.get() == &frame; });
	if (it == frame_list.end()) { throw std::invalid_argument("invalid desktop frame"); }
	frame_list.erase(it);
	if (frame_list.empty()) { Terminate(); }
}

Size Desktop::GetSize() const { return Win32::GetDesktopSize(); }

DesktopFrame& Desktop::GetDesktopFrame(WndObject& wnd) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window has no desktop frame"); }
	}
	return static_cast<DesktopFrame&>(*child);
}

DesktopFrame& Desktop::GetDesktopFramePoint(WndObject& wnd, Point& point) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window has no desktop frame"); }
		point += parent->GetChildOffset(*child);
	}
	return static_cast<DesktopFrame&>(*child);
}

void Desktop::RecreateFrameLayer() {
	for (auto& frame : frame_list) {
		frame->RecreateLayer();
	}
}

void Desktop::SetCapture(WndObject& wnd) {
	wnd_capture_offset = point_zero;
	DesktopFrame& frame = GetDesktopFramePoint(wnd, wnd_capture_offset);
	wnd_capture_offset += frame.point - point_zero;
	if (frame_capture != &frame) { Win32::SetCapture(frame.hwnd); }
	if (wnd_capture != &wnd) { LoseCapture(); }
	frame_capture = &frame; wnd_capture = &wnd; wnd_capture->is_mouse_captured = true;
}

void Desktop::ReleaseCapture(WndObject& wnd) {
	if (wnd_capture == &wnd) { Win32::ReleaseCapture(); }
}

void Desktop::LoseCapture() {
	if (wnd_capture != nullptr) {
		wnd_capture->is_mouse_captured = false;
		frame_capture = nullptr; wnd_capture = nullptr;
	}
}

void Desktop::LoseTrack(std::vector<ref_ptr<WndObject>>::iterator wnd_track_index_begin) {
	if (wnd_track_index_begin >= wnd_track_stack.end()) { return; };
	auto it_begin = wnd_track_index_begin, it_end = wnd_track_stack.end();
	for (auto it = it_end; it > it_begin;) {
		(*--it)->OnNotifyMsg(NotifyMsg::MouseLeave); (*it)->is_mouse_tracked = false;
	}
	wnd_track_stack.erase(it_begin, it_end);
}

void Desktop::DispatchMouseMsg(frame_ref frame, MouseMsg msg) {
	if (wnd_capture != nullptr) {
		msg.point += frame.point - wnd_capture_offset;
		return wnd_capture->OnMouseMsg(msg);
	}
	ref_ptr<WndObject> parent = &frame;
	auto wnd_track_index = wnd_track_stack.begin();
	do {
		if (wnd_track_index >= wnd_track_stack.end() || *wnd_track_index != parent) {
			LoseTrack(wnd_track_index);
			wnd_track_stack.push_back(parent); wnd_track_index = wnd_track_stack.end();
			parent->OnNotifyMsg(NotifyMsg::MouseEnter); parent->is_mouse_tracked = true;
		}
		ref_ptr<WndObject> child = parent->HitTest(msg.point);
		if (child == nullptr) { SetCursor(Cursor::Default); return; }
		if (child == parent) { return parent->OnMouseMsg(msg); }
		parent = child;
	} while (true);
}

void Desktop::SetFocus(WndObject& wnd) {
	DesktopFrame& frame = GetDesktopFrame(wnd);
	if (frame_focus != &frame) { Win32::SetFocus(frame.hwnd); }
	if (wnd_focus != &wnd) { LoseFocus(); }
	frame_focus = &frame; wnd_focus = &wnd; ime_focus = nullptr; wnd_focus->is_on_focus = true;
	if (wnd.ime_aware) {
		ime_focus = ime_wnd_map.at(&wnd);
		WndDesign::ImeEnable(frame.hwnd);
	} else {
		WndDesign::ImeDisable(frame.hwnd);
	}
}

void Desktop::LoseFocus() {
	if (wnd_focus != nullptr) {
		wnd_focus->OnNotifyMsg(NotifyMsg::LoseFocus);
		wnd_focus->is_on_focus = false;
		frame_focus = nullptr; wnd_focus = nullptr; ime_focus = nullptr;
	}
}

void Desktop::DispatchKeyMsg(frame_ref frame, KeyMsg msg) {
	if (wnd_focus != nullptr) {
		wnd_focus->OnKeyMsg(msg);
	}
}

void Desktop::ImeSetPosition(WndObject& wnd, Point point) {
	DesktopFrame& frame = GetDesktopFramePoint(wnd, point);
	WndDesign::ImeSetPosition(frame.hwnd, point);
}

void Desktop::MessageLoop() { Win32::MessageLoop(); }

void Desktop::Terminate() { frame_list.clear(); Win32::Terminate(); }


END_NAMESPACE(WndDesign)