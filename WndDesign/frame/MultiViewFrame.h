#pragma once

#include "../window/WndObject.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class MultiViewFrame;

class MultiViewPort : public WndObject {
	MultiViewFrame& frame;



};


class MultiViewFrame {


private:
	WndObject::child_ptr child;

	std::vector<ref_ptr<MultiViewPort>> viewports;
};


END_NAMESPACE(WndDesign)