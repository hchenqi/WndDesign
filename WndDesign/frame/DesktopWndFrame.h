#pragma once

#include "../window/wnd_traits.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


class DesktopWndFrame : private WndObject {
public:
	using child_ptr = child_ptr<Relative, Relative>;
public:
	DesktopWndFrame(child_ptr wnd) {

	}

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