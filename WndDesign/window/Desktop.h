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

public:
	Desktop();
	~Desktop();

	// layout
public:
	const Size GetSize() const;

	// frame
private:
	std::vector<Desktop::frame_ptr> frame_list;
public:
	void AddChild(frame_ptr frame);
	void RemoveChild(frame_ref frame);
public:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);

	// mouse capture
private:
	ref_ptr<DesktopFrame> frame_capture = nullptr;
	ref_ptr<WndObject> wnd_capture = nullptr;
	Point wnd_capture_offset;
public:
	void SetCapture(WndObject& wnd);
	void ReleaseCapture(WndObject& wnd);
	void LoseCapture();

	// mouse track
private:
	std::vector<ref_ptr<WndObject>> wnd_track_stack;
	ref_ptr<WndObject> wnd_mouse_receive = nullptr;
public:
	void LoseTrack(std::vector<ref_ptr<WndObject>>::iterator wnd_track_index_begin = {});

	// mouse message
public:
	void DispatchMouseMsg(frame_ref frame, MouseMsg msg);
	void PassMouseMsg(WndObject& wnd, MouseMsg msg);

	// key focus
private:
	ref_ptr<DesktopFrame> frame_focus = nullptr;
	ref_ptr<WndObject> wnd_focus = nullptr;
	ref_ptr<ImeMsgHandler> ime_focus = nullptr;
	ref_ptr<WndObject> wnd_key_receive = nullptr;
public:
	void SetFocus(WndObject& wnd);
	void LoseFocus();

	// key message
public:
	void DispatchKeyMsg(frame_ref frame, KeyMsg msg);
	void PassKeyMsg(WndObject& wnd, KeyMsg msg);

	// ime message
private:
	std::unordered_map<ref_ptr<WndObject>, ref_ptr<ImeMsgHandler>> ime_wnd_map;
public:
	void SetImeWnd(WndObject& wnd, ImeMsgHandler& ime) { ime_wnd_map.emplace(&wnd, &ime); ImeEnable(wnd); }
	void RemoveImeWnd(WndObject& wnd) { ime_wnd_map.erase(&wnd); }
public:
	void ImeEnable(WndObject& wnd) { wnd.ime_aware = true; }
	void ImeDisable(WndObject& wnd) { wnd.ime_aware = false; }
	void ImeSetPosition(WndObject& wnd, Point point);
public:
	void OnImeSetContext(frame_ref frame);
	void OnImeCompositionBegin() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionBegin(); } }
	void OnImeComposition(std::wstring str) { if (ime_focus != nullptr) { ime_focus->OnImeComposition(str); } }
	void OnImeCompositionEnd() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionEnd(); } }
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)