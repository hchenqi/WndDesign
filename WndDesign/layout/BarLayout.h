#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class BarLayoutHorizontal : public WndObject {

private:
	child_ptr child_left;
	child_ptr child_right;
	child_ptr child_center;

private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {

	}

};


END_NAMESPACE(WndDesign)