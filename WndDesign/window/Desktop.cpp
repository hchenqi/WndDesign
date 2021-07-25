#include "Desktop.h"
#include "../system/d2d_hwnd.h"


BEGIN_NAMESPACE(WndDesign)



void DesktopWndFrame::UpdateInvalidRegion() {
	FigureQueue figure_queue;
	OnPaint(figure_queue, GetInvalidRegion());
	target->DrawFigureQueue(figure_queue, vector_zero, Rect(point_zero, GetSize()));
}

void DesktopWndFrame::Present() { 

}


END_NAMESPACE(WndDesign)