#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


class OverlapFrame : private WndObject {
private:
	friend class OverlapLayout;
};


END_NAMESPACE(WndDesign)