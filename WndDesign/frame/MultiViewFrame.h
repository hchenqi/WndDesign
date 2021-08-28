#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class MultiViewFrame;

class ViewPort : public WndType<Auto, Auto> {
	MultiViewFrame& frame;



};


class MultiViewFrame : private WndObject {
public:
	using child_ptr = child_ptr<Auto, Auto>;
public:
	MultiViewFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }

private:
	child_ptr child;

	std::vector<ref_ptr<ViewPort>> viewports;

	std::unique_ptr<ViewPort> CreateViewPort() {

	}
};


END_NAMESPACE(WndDesign)