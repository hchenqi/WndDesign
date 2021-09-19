#pragma once

#include "../window/wnd_traits.h"
#include "../style/desktop_frame_style.h"
#include "../figure/desktop_layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : private WndObject {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	using child_ptr = child_ptr<Relative, Relative>;

public:
	DesktopFrame(DesktopFrameStyle style, child_ptr child);
	~DesktopFrame();

	// style
private:
	DesktopFrameStyle style;
private:
	const std::pair<Size, Rect> GetMinMaxRegion() const;
protected:
	void SetTitle(const std::wstring& title);

	// layout
private:
	Rect region;
	Rect client_region;
private:
	const Rect GetRegion() const { return region; }
	void SetRegion(Rect new_region);
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return child; }

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
	void Destroy();

	// paint
private:
	DesktopLayer layer;
	Region invalid_region;
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override;
	void Draw();
private:
	void RecreateLayer();
};


END_NAMESPACE(WndDesign)