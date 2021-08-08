#pragma once

#include "OverlapWndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopWndFrame : private OverlapWndFrame {
private:
	friend class Desktop;

private:
	void GetMinMaxSize() {}

	const Size UpdateLayout(Size size) {}
};


END_NAMESPACE(WndDesign)