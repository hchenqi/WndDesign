#include "DesktopFrame.h"
#include "../window/desktop.h"
#include "../style/style_helper.h"
#include "../figure/desktop_layer.h"
#include "../system/win32_api.h"
#include "../system/d2d_api.h"
#include "../system/directx_resource.h"


BEGIN_NAMESPACE(WndDesign)


DesktopFrame::DesktopFrame(DesktopFrameStyle style, child_ptr child) : style(style), child(std::move(child)), hwnd(nullptr) {
	RegisterChild(this->child);
	Rect region = StyleHelper::CalculateRegion(style.width, style.height, style.position, desktop.GetSize());
	hwnd = Win32::CreateWnd(region, style.title);
	layer.Create(hwnd, region.size);
}

DesktopFrame::~DesktopFrame() {
	layer.Destroy();
	Win32::DestroyWnd(hwnd);
}

const std::pair<Size, Rect> DesktopFrame::GetMinMaxRegion() const {
	auto [size_min, size_max] = StyleHelper::CalculateMinMaxSize(style.width, style.height, desktop.GetSize());
	Rect region_max(point_zero, size_max);
	if (size_max == desktop.GetSize()) {
		region_max = ExtendRegionByMargin(region_max, StyleHelper::CalculateBorderMargin(style.border));
	}
	return { size_min, region_max };
}

void DesktopFrame::SetTitle(const std::wstring& title) { Win32::SetWndTitle(hwnd, title); }

void DesktopFrame::SetRegion(Rect new_region) {
	if (new_region.size != region.size) {
		region.size = new_region.size;
		layer.Resize(new_region.size);
		client_region = ShrinkRegionByMargin(Rect(point_zero, region.size), StyleHelper::CalculateBorderMargin(style.border));
		UpdateChildSizeRef(child, client_region.size);
	}
	region.point = new_region.point;
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::OnChildRedraw(const WndObject& child, Rect redraw_region) {
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::Draw(Rect draw_region) {
	BeginDraw();
	FigureQueue figure_queue; figure_queue.Begin();
	DrawChild(child, point_zero, figure_queue, draw_region);
	figure_queue.End(); layer.DrawFigureQueue(figure_queue, vector_zero, draw_region);
	try {
		EndDraw();
	} catch (std::runtime_error&) {
		DirectXRecreateResource();
		layer.Create(hwnd, region.size);
		return Win32::InvalidateWndRegion(hwnd, Rect(point_zero, region.size));
	}
	layer.Present();
}


END_NAMESPACE(WndDesign)