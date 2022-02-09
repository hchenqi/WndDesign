#pragma once

#include "WndFrame.h"
#include "../style/style.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapFrame : protected WndFrame {
private:
	friend class OverlapLayout;

public:
	using Style = PositionStyle;
	using child_ptr = child_ptr<>;

public:
	OverlapFrame(Style style, child_ptr child) : WndFrame(std::move(child)), position(style) {}
	~OverlapFrame() {}

	// style
protected:
	PositionStyle position;

	// layout
private:
	Rect region = region_empty;
protected:
	Rect GetRegion() const { return region; }
private:
	Rect UpdateLayout(Size size, Size size_ref);
};


END_NAMESPACE(WndDesign)