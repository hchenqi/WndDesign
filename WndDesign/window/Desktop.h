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
	using frame_ptr = std::unique_ptr<DesktopFrame>;

private:
	Desktop();
	~Desktop();

public:
	static Desktop& Get();

	// frame
private:
	std::vector<frame_ptr> frame_list;
private:
	void AddChild(frame_ptr frame);
	void RemoveChild(DesktopFrame& frame);
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
	void SetCapture(WndObject& wnd);
	void ReleaseCapture();

	// key message
private:
	ref_ptr<DesktopFrame> frame_focus = nullptr;
	ref_ptr<WndObject> wnd_focus = nullptr;
private:
	void SetFocus(WndObject& wnd);
	void ReleaseFocus(WndObject& wnd);
	void LoseFocus();
private:
	void DispatchKeyMsg(KeyMsg msg) { if (wnd_focus != nullptr) { wnd_focus->OnKeyMsg(msg); } }

	// ime
private:
	std::unordered_set<ref_ptr<WndObject>> ime_enabled_wnd;
private:
	void ImeEnable(WndObject& wnd) { ime_enabled_wnd.emplace(&wnd); }
	void ImeDisable(WndObject& wnd) { ime_enabled_wnd.erase(&wnd); }
	void ImeSetPosition(WndObject& wnd, Point point);

	// global
private:
	void MessageLoop();
	void Terminate();
};

extern Desktop& desktop;


END_NAMESPACE(WndDesign)