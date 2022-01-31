#include "desktop.h"
#include "../frame/DesktopFrame.h"
#include "../system/win32_api.h"
#include "../system/win32_ime.h"


BEGIN_NAMESPACE(WndDesign)

Desktop desktop;


Desktop::Desktop() {}

Desktop::~Desktop() {}

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
		point *= parent->GetChildTransform(*child);
	}
	return static_cast<DesktopFrame&>(*child);
}

void Desktop::RecreateFrameLayer() {
	for (auto& frame : frame_list) {
		frame->RecreateLayer();
	}
}

void Desktop::SetCapture(WndObject& wnd) {
	Win32::SetCapture(static_cast<DesktopFrame&>(wnd).hwnd);
}

void Desktop::ReleaseCapture() {
	Win32::ReleaseCapture();
}

void Desktop::SetFocus(WndObject& wnd) {
	if (wnd_focus == &wnd) { return; }
	DesktopFrame& frame = GetDesktopFrame(wnd);
	if (frame_focus != &frame) { Win32::SetFocus(frame.hwnd); }
	if (wnd_focus != &wnd) { LoseFocus(); }
	frame_focus = &frame; wnd_focus = &wnd; ime_focus = nullptr;
	if (auto it = wnd_ime_map.find(&wnd); it != wnd_ime_map.end()) {
		ime_focus = it->second;
		WndDesign::ImeEnable(frame.hwnd);
	} else {
		WndDesign::ImeDisable(frame.hwnd);
	}
}

void Desktop::ReleaseFocus(WndObject& wnd) {
	if (wnd_focus == &wnd) {
		Win32::SetFocus(nullptr);
	}
}

void Desktop::LoseFocus() {
	if (wnd_focus != nullptr) {
		wnd_focus->OnNotifyMsg(NotifyMsg::LoseFocus);
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