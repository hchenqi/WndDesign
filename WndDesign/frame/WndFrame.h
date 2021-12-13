#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child(std::move(child)) { RegisterChild(*this->child); }
	virtual ~WndFrame() override {}
protected:
	child_ptr<> child;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { return SizeUpdated(child_size); }
protected:
	virtual Vector GetChildOffset(WndObject& child) override { return vector_zero; }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { return child; }
protected:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { return Redraw(redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { return DrawChild(child, point_zero, figure_queue, draw_region); }
};


template<class WidthType, class HeightType>
class WndFrameMutable : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	WndFrameMutable(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size_ref;
public:
	void SetChild(child_ptr child) { this->child = std::move(child); SizeUpdated(UpdateChildSizeRef(this->child, size_ref)); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { this->size_ref = size_ref; return UpdateChildSizeRef(child, size_ref); }
};


template<class T>
WndFrameMutable(T)->WndFrameMutable<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)