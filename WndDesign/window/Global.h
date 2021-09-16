#pragma once

#include "../geometry/geometry.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


struct Global {
	void AddWnd(std::unique_ptr<DesktopFrame> frame);
	void RemoveWnd(DesktopFrame& frame);
	void MessageLoop();
	void Terminate();
};

inline static Global global;


END_NAMESPACE(WndDesign)