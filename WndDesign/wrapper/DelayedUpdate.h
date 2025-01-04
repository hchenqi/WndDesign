#pragma once

#include "../frame/WndFrame.h"

#include <optional>


BEGIN_NAMESPACE(WndDesign)


template<class Frame> requires std::is_base_of_v<WndFrame, Frame>
class DelayedReflow : public Frame {
protected:
	using Base = DelayedReflow;

public:
	using Frame::Frame;

protected:
	std::optional<Size> current_child_size;
public:
	void Reflow() {
		if (current_child_size.has_value()) {
			Frame::OnChildSizeUpdate(Frame::child, current_child_size.value());
			current_child_size.reset();
		}
	}

protected:
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override { current_child_size = child_size; }
};


template<class Frame> requires std::is_base_of_v<WndFrame, Frame>
class DelayedRedraw : public Frame {
protected:
	using Base = DelayedRedraw;

public:
	using Frame::Frame;

protected:
	Rect current_child_redraw_region;
public:
	void Redraw() {
		if (!current_child_redraw_region.IsEmpty()) {
			Frame::OnChildRedraw(Frame::child, current_child_redraw_region);
			current_child_redraw_region = region_empty;
		}
	}

protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override { current_child_redraw_region = current_child_redraw_region.Union(child_redraw_region); }
};


END_NAMESPACE(WndDesign)