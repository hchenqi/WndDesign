#pragma once

#include "../frame/OverlapFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapLayout : public WndType<Assigned, Assigned> {
public:
	using frame_ptr = std::unique_ptr<OverlapFrame>;
	using frame_ref = OverlapFrame&;

public:
	OverlapLayout() {}
	~OverlapLayout() {}

	// child
protected:
	std::vector<frame_ptr> frame_list;
protected:
	static frame_ref AsFrame(WndObject& child) { return static_cast<frame_ref>(child); }
public:
	void AddChild(frame_ptr frame);
	void AddChild(alloc_ptr<OverlapFrame> frame) { AddChild(frame_ptr(frame)); }
	void RemoveChild(frame_ref frame);

	// layout
protected:
	Size size = size_empty;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)