#pragma once

#include "WndObject.h"
#include "../message/ime.h"

#include <memory>
#include <vector>
#include <unordered_map>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


class Desktop : public WndObject {
public:
	using frame_ptr = std::unique_ptr<DesktopFrame>;
	using frame_ref = DesktopFrame&;

	// layout
public:
	const Size GetSize() const;

	// desktop frame and window
private:
	std::vector<Desktop::frame_ptr> frame_list;
public:
	void AddChild(frame_ptr frame);
	void RemoveChild(frame_ref frame);
public:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);

	// mouse and key message
private:
	using WndTrackInfo = struct { ref_ptr<WndObject> wnd; Point point; };
	std::vector<WndTrackInfo> track_wnd_stack;
	ref_ptr<WndObject> wnd_capture;
	ref_ptr<WndObject> wnd_focus;
private:
	uint wnd_mouse_receive_index;
	ref_ptr<WndObject> wnd_key_receive;
public:
	void SetCapture(WndObject& wnd) {}
	void ReleaseCapture(WndObject& wnd) {}
	void SetFocus(WndObject& wnd) {
		if (wnd_focus != nullptr) {

		} else {
			Win32::SetFocus(GetDesktopFrame(wnd).hwnd);
		}
		ime_focus = nullptr; 
		if (wnd.ime_aware) {
			if (auto it = ime_wnd_map.find(&wnd); it != ime_wnd_map.end()) {
				ime_focus = it->second;
			}
		} 
	}
public:
	void LoseTrack() {

	}
	void LoseCapture() { wnd_capture = nullptr; }
	void LoseFocus() { 
		if (wnd_focus != nullptr) {
			wnd_focus->OnNotifyMsg(NotifyMsg::LoseFocus); 
			wnd_focus = nullptr; ime_focus = nullptr;
		}
	}
public:
	void DispatchMouseMsg(frame_ref frame, MouseMsg msg) {
		frame.HitTest(msg.point);



	}
	void DispatchKeyMsg(frame_ref frame, KeyMsg msg) {
		if (wnd_focus != nullptr) { wnd_focus->OnKeyMsg(msg); }
	}
public:
	void PassMouseMsg(WndObject& wnd, MouseMsg msg) {

	}
	void PassKeyMsg(WndObject& wnd, KeyMsg msg) {
		if (wnd_key_receive == &wnd) {
			wnd_key_receive = wnd.parent;
			if (wnd_key_receive != nullptr) {
				wnd_key_receive->OnKeyMsg(msg);
			}
		}
	}

	// ime message
private:
	std::unordered_map<ref_ptr<WndObject>, ref_ptr<ImeMsgHandler>> ime_wnd_map;
	ref_ptr<ImeMsgHandler> ime_focus = nullptr;
public:
	void SetImeWnd(WndObject& wnd, ImeMsgHandler& ime) { ime_wnd_map.emplace(&wnd, &ime); ImeEnable(wnd); }
	void RemoveImeWnd(WndObject& wnd) { ime_wnd_map.erase(&wnd); }
public:
	void ImeEnable(WndObject& wnd) { wnd.ime_aware = true; }
	void ImeDisable(WndObject& wnd) { wnd.ime_aware = false; }
	void ImeSetPosition(WndObject& wnd, Point point);
public:
	void OnImeSetContext(HANDLE hwnd);
	void OnImeCompositionBegin() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionBegin(); } }
	void OnImeComposition(std::wstring str) { if (ime_focus != nullptr) { ime_focus->OnImeComposition(str); } }
	void OnImeCompositionEnd() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionEnd(); } }
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)