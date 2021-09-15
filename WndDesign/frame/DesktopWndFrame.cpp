#include "DesktopWndFrame.h"
#include "../figure/desktop_wnd_layer.h"
#include "../system/win32_api.h"
#include "../system/d2d_api.h"


BEGIN_NAMESPACE(WndDesign)



void DesktopWndFrame::UpdateInvalidRegion() {
	FigureQueue figure_queue;
	OnPaint(figure_queue, GetInvalidRegion());
	target->DrawFigureQueue(figure_queue, vector_zero, Rect(point_zero, GetSize()));
}

void DesktopWndFrame::Present() {

}

void DesktopWndFrame::OnChildRedraw(const WndObject& child, Rect redraw_region) {
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopWndFrame::Draw(Rect draw_region) const {
	BeginDraw();
	FigureQueue figure_queue; figure_queue.Begin();
	DrawChild(child, point_zero, figure_queue, draw_region);
	figure_queue.End(); layer->DrawFigureQueue(figure_queue, vector_zero, draw_region);
	EndDraw();
	layer->Present();
}


END_NAMESPACE(WndDesign)