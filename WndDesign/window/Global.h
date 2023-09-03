#pragma once

#include "../geometry/geometry.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)

class WndObject;
class DesktopFrame;


struct Global {
	static DesktopFrame& AddWnd(std::unique_ptr<DesktopFrame> frame);
	static DesktopFrame& AddWnd(alloc_ptr<DesktopFrame> frame);
	static std::unique_ptr<DesktopFrame> RemoveWnd(DesktopFrame& frame);

	static DesktopFrame& GetDesktopFrame(WndObject& wnd);
	static DesktopFrame& GetDesktopFramePoint(WndObject& wnd, Point& point);

	static void MessageLoop();
	static void Terminate();
};

constexpr Global global;


END_NAMESPACE(WndDesign)