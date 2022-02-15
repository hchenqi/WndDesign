#pragma once

#include "../geometry/geometry.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)

class DesktopFrame;


struct Global {
	static void AddWnd(std::unique_ptr<DesktopFrame> frame);
	static void AddWnd(alloc_ptr<DesktopFrame> frame);
	static void RemoveWnd(DesktopFrame& frame);
	static void MessageLoop();
	static void Terminate();
};

constexpr Global global;


END_NAMESPACE(WndDesign)