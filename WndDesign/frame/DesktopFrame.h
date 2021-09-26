#pragma once

#include "BorderFrame.h"
#include "../style/desktop_frame_style.h"
#include "../figure/desktop_layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : private BorderFrame<Assigned, Assigned> {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	DesktopFrame(DesktopFrameStyle style, child_ptr child);
	~DesktopFrame();

	// style
private:
	DesktopFrameStyle style;
private:
	std::pair<Size, Rect> GetMinMaxRegion() const;
protected:
	void SetTitle(std::wstring title);

	// layout
private:
	Point point;
private:
	Rect GetRegion() { return Rect(point, size); }
	void SetRegion(Rect new_region);

	// hwnd
protected:
	using HANDLE = void*;
	HANDLE hwnd = nullptr;
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
	void ResizeLayer();
	void RecreateLayer();
private:
	void Redraw(Rect redraw_region);
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override;
	void Draw();

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
};


END_NAMESPACE(WndDesign)