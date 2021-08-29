#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
public:
	using child_ptr = child_ptr<>;
protected:
	WndFrame(child_ptr child) : child(std::move(child)) { RegisterChild(*this->child); }
protected:
	child_ptr child;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override { SizeUpdated(child_size); }
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override { Redraw(redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override { DrawChild(child, point_zero, figure_queue, draw_region); }
private:
	virtual void OnMouseMsg(MouseMsg& msg) override { SendChildMouseMsg(child, msg); }
};


class WndFrameMutable : public WndFrame {
public:
	WndFrameMutable(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size_ref;
public:
	void SetChild(child_ptr child) { this->child = std::move(child); SizeUpdated(UpdateChildSizeRef(this->child, size_ref)); }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { this->size_ref = size_ref; return UpdateChildSizeRef(child, size_ref); }
};


END_NAMESPACE(WndDesign)