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
	ref_ptr<ImeApi> ime_focus = nullptr;
public:
	void SetFocus(WndObject& wnd);
	void ReleaseFocus(WndObject& wnd);
	void LoseFocus();
public:
	void DispatchKeyMsg(DesktopFrame& frame, KeyMsg msg);

	// ime message
private:
	std::unordered_map<ref_ptr<WndObject>, ref_ptr<ImeApi>> wnd_ime_map;
public:
	void ImeEnable(WndObject& wnd, ImeApi& ime) { wnd_ime_map.emplace(&wnd, &ime); }
	void ImeDisable(WndObject& wnd) { wnd_ime_map.erase(&wnd); }
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