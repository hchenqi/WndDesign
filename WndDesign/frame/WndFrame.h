#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
protected:
	WndFrame(child_ptr<> child) : child(std::move(child)) { RegisterChild(this->child); }
	virtual ~WndFrame() override {}

	// child
protected:
	child_ptr<> child;
public:
	WndObject& GetChild() { return child; }

	// layout
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return Transform::Identity(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return UpdateChildSizeRef(child, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { return SizeUpdated(child_size); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { Redraw(child_redraw_region); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override { DrawChild(child, point_zero, figure_queue, draw_region); }

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override { return child; }
};


class WndFrameMutable : public WndFrame {
public:
	WndFrameMutable(child_ptr<> child) : WndFrame(std::move(child)) {}

	// child
public:
	child_ptr<> Reset(child_ptr<> child) {
		UnregisterChild(this->child);
		child.swap(this->child);
		RegisterChild(this->child);
		SizeUpdated(UpdateChildSizeRef(this->child, size_ref));
		return child;
	}

	// layout
private:
	Size size_ref;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		return WndFrame::OnSizeRefUpdate(this->size_ref = size_ref);
	}
};


END_NAMESPACE(WndDesign)