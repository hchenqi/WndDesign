#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapWndFrame : private WndObject {
private:
	friend class OverlapLayout;

	const Point GetPosition() const {

	}
};


END_NAMESPACE(WndDesign)