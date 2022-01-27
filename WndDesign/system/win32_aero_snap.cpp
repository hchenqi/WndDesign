#include "win32_aero_snap.h"
#include "../window/desktop.h"
#include "../frame/DesktopFrame.h"

#include <Windows.h>


BEGIN_NAMESPACE(WndDesign)

struct DesktopFrameApi : DesktopFrame {
	DesktopFrame::hwnd;
};


void AeroSnapDraggingEffect(WndObject& wnd) {
	HANDLE hwnd = static_cast<DesktopFrameApi&>(desktop.GetDesktopFrame(wnd)).hwnd;
	SendMessageW((HWND)hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void AeroSnapBorderResizingEffect(WndObject& wnd, BorderPosition border_position) {
	HANDLE hwnd = static_cast<DesktopFrameApi&>(desktop.GetDesktopFrame(wnd)).hwnd;
	SendMessageW((HWND)hwnd, WM_NCLBUTTONDOWN, (WPARAM)border_position, 0);
}


END_NAMESPACE(WndDesign)