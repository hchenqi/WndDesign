#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class BarLayoutHorizontal : public WndObject {

private:
	ref_ptr<WndObject> child_left;
	ref_ptr<WndObject> child_right;
	ref_ptr<WndObject> child_center;

private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {

	}

};


END_NAMESPACE(WndDesign)