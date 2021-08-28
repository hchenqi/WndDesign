#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


enum class NotifyMsg : uchar {
	MouseEnter,
	MouseLeave,
	LoseCapture,
	LoseFocus,
};


END_NAMESPACE(WndDesign)