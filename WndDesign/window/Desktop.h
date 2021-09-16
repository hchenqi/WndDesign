#pragma once

#include "../geometry/geometry.h"
#include "../message/message.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


class Desktop : public WndObject {
public:
	using frame_ptr = std::unique_ptr<DesktopFrame>;
	using frame_ref = DesktopFrame&;

private:
	std::vector<Desktop::frame_ptr> frame_list;
public:
	void AddChild(frame_ptr frame);
	void RemoveChild(frame_ref frame);

private:
	struct TrackWnd {
		ref_ptr<WndObject> wnd;
		Point point;
	};
	std::vector<TrackWnd> track_wnd_stack;
	ref_ptr<WndObject> wnd_capture;
	ref_ptr<WndObject> wnd_focus;
private:
	uint wnd_mouse_receive_index;
	ref_ptr<WndObject> wnd_key_receive;

public:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);

public:
	void SetCapture(WndObject& wnd) {}
	void ReleaseCapture(WndObject& wnd) {}
	void SetFocus(WndObject& wnd) {}
public:
	void LoseTrack() { 
	
	}
	void LoseCapture() { wnd_capture = nullptr; }
	void LoseFocus() { if (wnd_focus != nullptr) { wnd_focus->OnNotifyMsg(NotifyMsg::LoseFocus); wnd_focus = nullptr; } }
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
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)