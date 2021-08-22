#pragma once

#include "OverlapWndFrame.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


class DesktopWndFrame : private OverlapWndFrame {

	struct {

	} initial_margin;



private:
	friend class Desktop;

private:
	using HANDLE = void*;
	class HWNDTarget;

	HANDLE hwnd;
	std::unique_ptr<HWNDTarget> target;

public:
	DesktopWndFrame(WndObject& wnd) {

	}


public:
	void UpdateInvalidRegion();
	void Present();
	void RecreateTarget();


private:
	void GetMinMaxSize() {}

	virtual const Size OnSizeRefUpdate(Size size_ref) override {

	}

	const Size UpdateLayout(Size size) {}
};


END_NAMESPACE(WndDesign)