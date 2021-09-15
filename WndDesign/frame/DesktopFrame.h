#pragma once

#include "../window/wnd_traits.h"
#include "../style/desktop_frame_style.h"

#include <string>
#include <vector>


BEGIN_NAMESPACE(WndDesign)

class DesktopLayer;


class DesktopFrame : private WndObject {
private:
	friend class DesktopImpl;
	friend struct DesktopFrameApi;

public:
	using child_ptr = child_ptr<Relative, Relative>;

private:
	DesktopFrame(DesktopFrameStyle style, child_ptr child);
	~DesktopFrame();

	// style
private:
	DesktopFrameStyle style;
private:
	const std::pair<Size, Rect> GetMinMaxRegion() const;
protected:
	void SetTitle(const wstring& title) {}

	// layout
private:
	Rect region;
private:
	const Rect GetRegion() const {}
	void SetRegion(Rect new_region);

	// child
private:
	child_ptr child;

	// hwnd
protected:
	using HANDLE = void*;
	HANDLE hwnd;
protected:
	void Show();
	void Minimize();
	void Maximize();
	void Restore();

	// paint
private:
	std::unique_ptr<DesktopLayer> layer;
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override;
	void Draw(Rect draw_region) const;

	// message
private:
	void DispatchMouseMsg(MouseMsg msg) {}
	void DispatchKeyMsg(KeyMsg msg) {}
	void DispatchNotifyMsg(NotifyMsg msg) {}
};


END_NAMESPACE(WndDesign)