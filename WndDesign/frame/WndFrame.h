#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class WndFrame : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	WndFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
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


template<class WidthType, class HeightType>
class WndFrameMutable : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	WndFrameMutable(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	Size size_ref;
	child_ptr child;
public:
	void SetChild(child_ptr child) { this->child = std::move(child); SizeUpdated(UpdateChildSizeRef(child, size_ref)); }
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { this->size_ref = size_ref; return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override { SizeUpdated(child_size); }
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override { Redraw(redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override { DrawChild(child, point_zero, figure_queue, draw_region); }
private:
	virtual void OnMouseMsg(MouseMsg& msg) override { SendChildMouseMsg(child, msg); }
};


END_NAMESPACE(WndDesign)