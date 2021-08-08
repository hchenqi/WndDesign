#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapWndFrame : private WndFrame {
private:
	friend class OverlapLayout;

	const Point GetPosition() const {

	}
};


END_NAMESPACE(WndDesign)