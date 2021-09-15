#include "DesktopFrame.h"
#include "../window/desktop_impl.h"
#include "../figure/desktop_wnd_layer.h"
#include "../style/style_helper.h"
#include "../system/win32_api.h"
#include "../system/d2d_api.h"


BEGIN_NAMESPACE(WndDesign)


DesktopFrame::DesktopFrame(DesktopFrameStyle style, child_ptr child) : style(style), child(std::move(child)), hwnd(nullptr) { 
	RegisterChild(this->child); 
	Rect region = StyleHelper::CalculateRegion(style.width, style.height, style.position, desktop_impl.GetSize());
	hwnd = Win32::CreateWnd(region, style.title._text);
	layer = std::make_unique<DesktopLayer>(hwnd, region.size);
}

DesktopFrame::~DesktopFrame() {
	layer.reset();
	Win32::DestroyWnd(hwnd);
}

const std::pair<Size, Rect> DesktopFrame::GetMinMaxRegion() const {
	return StyleHelper::CalculateMinMaxRegion(style.width, style.height, style.position, desktop_impl.GetSize());
}

void DesktopFrame::SetRegion(Rect new_region) {
	if (new_region.size != region.size) {
		region.size = new_region.size;
		layer->OnResize(new_region.size);
		UpdateChildSizeRef(child, GetChildSize());
	}
	region.point = new_region.point;
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }

void DesktopFrame::OnChildRedraw(const WndObject& child, Rect redraw_region) {
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::Draw(Rect draw_region) const {
	BeginDraw();
	FigureQueue figure_queue; figure_queue.Begin();
	DrawChild(child, point_zero, figure_queue, draw_region);
	figure_queue.End(); layer->DrawFigureQueue(figure_queue, vector_zero, draw_region);
	EndDraw();
	layer->Present();
}


END_NAMESPACE(WndDesign)