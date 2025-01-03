#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


enum class NotifyMsg : uchar {
	MouseEnter,
	MouseLeave,
	MouseOver,
	MouseOut,
	FocusIn,
	FocusOut,
	Focus,
	Blur,
};


END_NAMESPACE(WndDesign)