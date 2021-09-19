#include "win32_aero_snap.h"
#include "../window/desktop.h"
#include "../frame/DesktopFrame.h"

#include <Windows.h>


BEGIN_NAMESPACE(WndDesign)

struct DesktopFrameApi : DesktopFrame {
	DesktopFrame::hwnd;
};


void AeroSnapDraggingEffect(WndObject& wnd, Point point) {
	HANDLE hwnd = static_cast<DesktopFrameApi&>(desktop.GetDesktopFramePoint(wnd, point)).hwnd;
	SendMessageW((HWND)hwnd, WM_NCLBUTTONDOWN, HTCAPTION, ((short)point.y << 16) | (short)point.x);
}

void AeroSnapBorderResizingEffect(WndObject& wnd, Point point, BorderPosition border_position) {
	HANDLE hwnd = static_cast<DesktopFrameApi&>(desktop.GetDesktopFramePoint(wnd, point)).hwnd;
	SendMessageW((HWND)hwnd, WM_NCLBUTTONDOWN, (WPARAM)border_position, ((short)point.y << 16) | (short)point.x);
}


END_NAMESPACE(WndDesign)