#pragma once

#include "WndObject.h"
#include "../frame/DesktopFrame.h"
#include "../message/ime.h"

#include <memory>
#include <vector>
#include <unordered_map>


BEGIN_NAMESPACE(WndDesign)


class Desktop : public WndObject {
public:
	using frame_ptr = std::unique_ptr<DesktopFrame>;
	using frame_ref = DesktopFrame&;

public:
	Desktop() {}
	~Desktop() {}

	// layout
public:
	Size GetSize() const;

	// frame
private:
	std::vector<Desktop::frame_ptr> frame_list;
public:
	void AddChild(frame_ptr frame);
	void RemoveChild(frame_ref frame);
public:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);
public:
	void RecreateFrameLayer();

	// mouse message
private:
	ref_ptr<DesktopFrame> frame_capture = nullptr;
	ref_ptr<WndObject> wnd_capture = nullptr;
	Point wnd_capture_offset;
public:
	void SetCapture(WndObject& wnd);
	void ReleaseCapture(WndObject& wnd);
	void LoseCapture();
private:
	std::vector<ref_ptr<WndObject>> wnd_track_stack;
private:
	void LoseTrack(std::vector<ref_ptr<WndObject>>::iterator wnd_track_index_begin);
public:
	void LoseTrack() { LoseTrack(wnd_track_stack.begin()); }
public:
	void DispatchMouseMsg(frame_ref frame, MouseMsg msg);

	// key message
private:
	ref_ptr<DesktopFrame> frame_focus = nullptr;
	ref_ptr<WndObject> wnd_focus = nullptr;
	ref_ptr<ImeApi> ime_focus = nullptr;
public:
	void SetFocus(WndObject& wnd);
	void LoseFocus();
public:
	void DispatchKeyMsg(frame_ref frame, KeyMsg msg);

	// ime message
private:
	std::unordered_map<ref_ptr<WndObject>, ref_ptr<ImeApi>> ime_wnd_map;
public:
	void SetImeWnd(WndObject& wnd, ImeApi& ime) { ime_wnd_map.emplace(&wnd, &ime); ImeEnable(wnd); }
	void RemoveImeWnd(WndObject& wnd) { ime_wnd_map.erase(&wnd); }
public:
	void ImeEnable(WndObject& wnd) { wnd.ime_aware = true; }
	void ImeDisable(WndObject& wnd) { wnd.ime_aware = false; }
	void ImeSetPosition(WndObject& wnd, Point point);
public:
	void OnImeCompositionBegin() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionBegin(); } }
	void OnImeComposition(std::wstring str) { if (ime_focus != nullptr) { ime_focus->OnImeComposition(str); } }
	void OnImeCompositionEnd() { if (ime_focus != nullptr) { ime_focus->OnImeCompositionEnd(); } }

	// global
public:
	void MessageLoop();
	void Terminate();
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)