#pragma once

#include "../window/wnd_traits.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


class DesktopWndFrame : private WndObject {
public:
	using child_ptr = child_ptr<Relative, Relative>;
public:
	DesktopWndFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
private:
	using HANDLE = void*;
	class HWNDTarget;
	HANDLE hwnd;
	std::unique_ptr<HWNDTarget> target;
public:
	void UpdateInvalidRegion();
	void Present();
	void RecreateTarget();
private:
	void GetMinMaxSize() {}

private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override { SizeUpdated(child_size); }
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override { Redraw(redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override { DrawChild(child, point_zero, figure_queue, draw_region); }
private:
	virtual void OnMouseMsg(MouseMsg& msg) override { SendChildMouseMsg(child, msg); }
};


END_NAMESPACE(WndDesign)