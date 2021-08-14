#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrameEmpty : public WndObject {


private:
	WndObject& child;


private:
	virtual const Size OnSizeRefChange(Size size_ref) {
		return SetChildSizeRef(child, size_ref);
	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
		SizeChanged(child_size);
	}
};


END_NAMESPACE(WndDesign)