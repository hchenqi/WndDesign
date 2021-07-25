#pragma once

#include "WndFrame.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)

class HWNDTarget;
using HANDLE = void*;


class DesktopWndFrame : public WndFrame {
private:
	HANDLE hwnd;
	std::unique_ptr<HWNDTarget> target;

public:
	DesktopWndFrame(WndObject& wnd) {

	}


public:
	void UpdateInvalidRegion();
	void Present();
	void RecreateTarget();
};


class DesktopObject final : public WndObject {



	void AddChild(DesktopWndFrame& frame) {

	}
};


END_NAMESPACE(WndDesign)