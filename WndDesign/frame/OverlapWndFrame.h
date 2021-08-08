#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapWndFrame : private WndFrame {
private:
	friend class OverlapLayout;


	struct Style : public WndFrame::Style {
		struct MarginStyle {

		} margin;



		struct CompositeStyle {

		} composite;
	};

	const Point GetPosition() const {

	}
};


END_NAMESPACE(WndDesign)