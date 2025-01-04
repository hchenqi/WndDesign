#pragma once

#include "WndObject.h"

#include <memory>
#include <vector>
#include <unordered_set>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


class Desktop : public WndObject {
private:
	friend class DesktopFrame;
	friend class WndObject;
	friend struct Ime;
	friend struct DesktopApi;
	friend struct Global;

private:
	Desktop();
	~Desktop();

public:
	static Desktop& Get();

	// frame
private:
	std::vector<std::unique_ptr<DesktopFrame>> frame_list;
private:
	DesktopFrame& AddChild(std::unique_ptr<DesktopFrame> frame);
	std::unique_ptr<DesktopFrame> RemoveChild(DesktopFrame& frame);
private:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);

	// paint
private:
	void RecreateFrameLayer();
private:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;

	// mouse message
private:
	std::vector<ref_ptr<WndObject>> wnd_track_stack;
	std::unordered_map<ref_ptr<WndObject>, size_t> wnd_track_map;
	ref_ptr<DesktopFrame> frame_capture = nullptr;
	ref_ptr<WndObject> wnd_capture = nullptr;
private:
	void SetTrack(WndObject& wnd);
	void LoseTrack();
	void SetCapture(WndObject& wnd);
	void ReleaseCapture(WndObject& wnd);
	void LoseCapture();
private:
	void DispatchMouseMsg(DesktopFrame& frame, MouseMsg msg);

	// key message
private:
	std::vector<ref_ptr<WndObject>> wnd_focus_stack;
	std::unordered_map<ref_ptr<WndObject>, size_t> wnd_focus_map;
	ref_ptr<DesktopFrame> frame_focus = nullptr;
private:
	void SetFocus(WndObject& wnd);
	void ReleaseFocus(WndObject& wnd);
	void LoseFocus();
private:
	void DispatchKeyMsg(KeyMsg msg);

	// ime
private:
	std::unordered_set<ref_ptr<WndObject>> ime_enabled_wnd;
private:
	void ImeEnable(WndObject& wnd) { ime_enabled_wnd.emplace(&wnd); }
	void ImeDisable(WndObject& wnd) { ime_enabled_wnd.erase(&wnd); }
	void ImeSetPosition(WndObject& wnd, Point point);

	// message
private:
	void ReleaseWindow(WndObject& wnd);

	// global
private:
	void MessageLoop();
	void Terminate();
};

extern Desktop& desktop;


END_NAMESPACE(WndDesign)