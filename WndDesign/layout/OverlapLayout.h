#pragma once

#include "../frame/WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapFrame : protected WndFrame {
private:
	friend class OverlapLayout;

public:
	using child_ptr = child_ptr<>;

public:
	OverlapFrame(child_ptr child) : WndFrame(std::move(child)) {}
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
	virtual Rect OnOverlapFrameSizeRefUpdate(Size size_ref) { return Rect(point_zero, size_ref); }
private:
	void SizeUpdated() {}  // never used
	virtual Size OnSizeRefUpdate(Size size_ref) override final { return size_ref; }  // never used
};


class OverlapLayout : public WndType<Assigned, Assigned> {
private:
	friend class OverlapFrame;

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
	frame_ptr RemoveChild(frame_ref frame);

	// layout
protected:
	Size size = size_empty;
protected:
	void UpdateOverlapFrameChildSizeRef(frame_ref& frame, Size size_ref) { VerifyChild(frame); frame.region = frame.OnOverlapFrameSizeRefUpdate(size_ref); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnOverlapFrameChildRegionUpdate(WndObject& child, Rect child_region);
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;
private:
	void UpdateChildSizeRef() {}  // never used
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override final {}  // never used

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


inline OverlapLayout& OverlapFrame::GetParent() const { return static_cast<OverlapLayout&>(WndObject::GetParent()); }
inline void OverlapFrame::OverlapFrameRegionUpdated(Rect region) { if (HasParent()) { GetParent().OnOverlapFrameChildRegionUpdate(*this, region); } }


END_NAMESPACE(WndDesign)