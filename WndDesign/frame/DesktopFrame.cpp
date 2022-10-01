#include "DesktopFrame.h"
#include "../window/desktop.h"
#include "../figure/desktop_layer.h"
#include "../geometry/helper.h"
#include "../system/win32_api.h"
#include "../system/d2d_api.h"
#include "../system/directx_resource.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

inline float RoundWin32Length(float length) { length = ceilf(length); return length < 14.0f ? 14.0f : length; }

END_NAMESPACE(Anonymous)


DesktopFrame::DesktopFrame(std::wstring title, child_ptr<> child) : WndFrame(std::move(child)) {
	hwnd = Win32::CreateWnd(region_empty, title);
	Win32::SetWndUserData(hwnd, this);
	scale = Scale(Win32::GetWndDpiScale(hwnd));
}

DesktopFrame::~DesktopFrame() {
	layer.Destroy();
	Win32::SetWndUserData(hwnd, nullptr);
	Win32::DestroyWnd(hwnd);
}

void DesktopFrame::SetTitle(std::wstring title) { Win32::SetWndTitle(hwnd, title); }

void DesktopFrame::SetSize(Size size) {
	if (region.size != size) {
		region.size = size;
		UpdateChildSizeRef(child, size * scale.Invert());
		ResizeLayer();
	}
}

std::pair<Size, Rect> DesktopFrame::GetMinMaxRegion(Size size_ref) {
	auto [size_min, size_max] = CalculateMinMaxSize(size_ref * scale.Invert());
	return { size_min * scale, Rect(point_zero, size_max * scale) };
}

void DesktopFrame::InitializeRegion(Size size_ref) {
	region = OnDesktopFrameSizeRefUpdate(size_ref * scale.Invert()) * scale;
	region.size = Size(RoundWin32Length(region.size.width), RoundWin32Length(region.size.height));
	Win32::SetWndRegion(hwnd, region);
	RecreateLayer();
}

void DesktopFrame::DesktopFrameRegionUpdated(Rect region) {
	region = region * scale;
	region.size = Size(RoundWin32Length(region.size.width), RoundWin32Length(region.size.height));
	if (this->region.size != region.size) {
		this->region.size = region.size;
		ResizeLayer();
	}
	Win32::SetWndRegion(hwnd, region);
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::ResizeLayer() {
	layer.Resize(region.size);
	invalid_region.Clear();
	Redraw(region_infinite);
}

void DesktopFrame::RecreateLayer() {
	layer.Create(hwnd, region.size);
	invalid_region.Clear();
	Redraw(region_infinite);
}

void DesktopFrame::Redraw(Rect redraw_region) {
	redraw_region = Extend(redraw_region, 0.5f).Intersect(Rect(point_zero, region.size));
	invalid_region.Union(redraw_region);
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::Draw() {
	Rect render_rect = ceil(invalid_region.GetBoundingRect()); if (render_rect.IsEmpty()) { return; }
	BeginDraw();
	FigureQueue figure_queue([&](FigureQueue& figure_queue) { OnDraw(figure_queue, render_rect); });
	layer.DrawFigureQueue(figure_queue, vector_zero, render_rect);
	try {
		EndDraw();
	} catch (std::runtime_error&) {
		DirectXRecreateResource();
		return desktop.RecreateFrameLayer();
	}
	invalid_region.Clear();
	layer.Present(render_rect);
}

void DesktopFrame::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Redraw(child_redraw_region * scale);
}

void DesktopFrame::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	figure_queue.Group(scale, region_infinite, [&]() {
		DrawChild(child, point_zero, figure_queue, draw_region * scale.Invert());
	});
}


END_NAMESPACE(WndDesign)