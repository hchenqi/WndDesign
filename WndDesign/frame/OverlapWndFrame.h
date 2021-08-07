#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapWndFrame : public WndFrame {
	struct Style : public WndFrame::Style {
		struct MarginStyle {

		} margin;
	};


private:
	friend class OverlapLayout;

	const Rect UpdateLayoutRegion(Size size) {

	}
};


END_NAMESPACE(WndDesign)