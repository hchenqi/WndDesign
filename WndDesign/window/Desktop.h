#pragma once

#include "WndObject.h"

#include <memory>
#include <vector>
#include <unordered_set>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


class Desktop : public WndObject {
public:
	using frame_ptr = std::unique_ptr<DesktopFrame>;

public:
	Desktop();
	~Desktop();

	// frame
private:
	std::vector<frame_ptr> frame_list;
public:
	void AddChild(frame_ptr frame);
	void RemoveChild(DesktopFrame& frame);
public:
	DesktopFrame& GetDesktopFrame(WndObject& wnd);
	DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);
public:
	void RecreateFrameLayer();

	// paint
private:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;

	// mouse message
public:
	void SetCapture(WndObject& wnd);
	void ReleaseCapture();

	// key message
private:
	ref_ptr<DesktopFrame> frame_focus = nullptr;
	ref_ptr<WndObject> wnd_focus = nullptr;
public:
	void SetFocus(WndObject& wnd);
	void ReleaseFocus(WndObject& wnd);
	void LoseFocus();
public:
	void DispatchKeyMsg(KeyMsg msg) { if (wnd_focus != nullptr) { wnd_focus->OnKeyMsg(msg); } }

	// ime
private:
	std::unordered_set<ref_ptr<WndObject>> ime_enabled_wnd;
	std::wstring ime_string;
public:
	void ImeEnable(WndObject& wnd) { ime_enabled_wnd.emplace(&wnd); }
	void ImeDisable(WndObject& wnd) { ime_enabled_wnd.erase(&wnd); }
	void ImeSetPosition(WndObject& wnd, Point point);
public:
	void ImeSetString(std::wstring str) { ime_string = str; }
	std::wstring ImeGetString() { return std::move(ime_string); }

	// global
public:
	void MessageLoop();
	void Terminate();
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)