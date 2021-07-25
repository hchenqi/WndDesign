#pragma once

#include "WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {



private:
	WndObject& child;

	Scrollbar scrollbar_right;
	Border border;
	Padding padding;
};


END_NAMESPACE(WndDesign)