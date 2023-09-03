#pragma once

#include "../frame/WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapFrame : protected WndFrame {
private:
	friend class OverlapLayout;

public:
	OverlapFrame(child_ptr<Relative, Relative> child) : WndFrame(std::move(child)) {}
	~OverlapFrame() {}

	// parent
protected:
	OverlapLayout& GetParent() const;

	// layout
private:
	Rect region;  // only accessed by OverlapLayout
protected:
	void OverlapFrameRegionUpdated(Rect region);
protected:
	virtual Rect OnOverlapFrameSizeRefUpdate(Size size_ref) { UpdateChildSizeRef(child, size_ref); return Rect(point_zero, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
private:
	void SizeUpdated() {}  // never used
	virtual Size OnSizeRefUpdate(Size size_ref) override final { return size_ref; }  // never used
};


class OverlapLayout : public WndType<Assigned, Assigned> {
private:
	friend class OverlapFrame;

public:
	using frame_ptr = std::unique_ptr<OverlapFrame>;

public:
	OverlapLayout() {}
	~OverlapLayout() {}

	// child
protected:
	std::vector<frame_ptr> frame_list;
protected:
	static OverlapFrame& AsFrame(WndObject& child) { return static_cast<OverlapFrame&>(child); }
public:
	void AddChild(frame_ptr frame);
	void AddChild(alloc_ptr<OverlapFrame> frame) { AddChild(frame_ptr(frame)); }
	frame_ptr RemoveChild(OverlapFrame& frame);

	// layout
protected:
	Size size = size_empty;
protected:
	void UpdateOverlapFrameChildSizeRef(OverlapFrame& frame, Size size_ref) { VerifyChild(frame); frame.region = frame.OnOverlapFrameSizeRefUpdate(size_ref); }
protected:
	virtual Transform GetChildTransform(WndObject& child) const override;
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnOverlapFrameChildRegionUpdate(WndObject& child, Rect child_region);
private:
	void UpdateChildSizeRef() {}  // never used
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override final {}  // never used

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override;
};


inline OverlapLayout& OverlapFrame::GetParent() const { return static_cast<OverlapLayout&>(WndObject::GetParent()); }
inline void OverlapFrame::OverlapFrameRegionUpdated(Rect region) { if (HasParent()) { GetParent().OnOverlapFrameChildRegionUpdate(*this, region); } }


END_NAMESPACE(WndDesign)