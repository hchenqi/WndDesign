#pragma once

#include "OverlapWndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopWndFrame : private OverlapWndFrame {
private:
	friend class Desktop;

	struct Style : public OverlapWndFrame::Style {
		struct MenuBar {
		public:
			MenuBar& height(ValuePixel height) { return *this; }
			MenuBar& title(const wstring& title) { return *this; }
			MenuBar& minimizeButton() { return *this; }
			MenuBar& maximizeButton() { return *this; }
			MenuBar& closeButton() { return *this; }
		} menu_bar;

		struct Win32 {
		public:
			Win32& useNativeFrame() { return *this; }
			Win32& setToolWindow() { return *this; }
		} win32;
	};



private:
	void GetMinMaxSize() {}

	const Size UpdateLayout(Size size) {}
};


END_NAMESPACE(WndDesign)