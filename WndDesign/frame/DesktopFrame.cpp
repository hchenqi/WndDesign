#include "DesktopFrame.h"
#include "../window/desktop.h"
#include "../style/style_helper.h"
#include "../style/border_helper.h"
#include "../figure/desktop_layer.h"
#include "../figure/shape.h"
#include "../system/win32_api.h"
#include "../system/win32_aero_snap.h"
#include "../system/cursor.h"
#include "../system/d2d_api.h"
#include "../system/directx_resource.h"


BEGIN_NAMESPACE(WndDesign)


DesktopFrame::DesktopFrame(Style style, child_ptr child) :
	ScaleFrame({}, new BorderFrame(style.border, std::move(child))), width(style.width), height(style.height) {
	cursor = Cursor::Hide;
	hwnd = Win32::CreateWnd(region_empty, style.title);
	scale = Scale(Win32::GetWndDpiScale(hwnd));
	Size desktop_size = Win32::GetDesktopSize() * scale.Invert();
	region = StyleHelper::CalculateRegion(style.width, style.height, style.position, desktop_size) * scale;
	OnSizeRefUpdate(region.size);
	Win32::SetWndRegion(hwnd, region);
	Win32::SetWndUserData(hwnd, this);
	RecreateLayer();
}

DesktopFrame::~DesktopFrame() {
	layer.Destroy();
	Win32::SetWndUserData(hwnd, nullptr);
	Win32::DestroyWnd(hwnd);
}

std::pair<Size, Rect> DesktopFrame::GetMinMaxRegion() const {
	Size desktop_size = Win32::GetDesktopSize() * scale.Invert();
	auto [size_min, size_max] = StyleHelper::CalculateMinMaxSize(width, height, desktop_size);
	return { size_min * scale, Rect(point_zero, size_max * scale) };
}

void DesktopFrame::SetTitle(std::wstring title) { Win32::SetWndTitle(hwnd, title); }

void DesktopFrame::SetSize(Size size) {
	if (region.size != size) {
		region.size = size;
		OnSizeRefUpdate(region.size);
		ResizeLayer();
	}
}

void DesktopFrame::SetStatus(Status status) {
	this->status = status;
	switch (status) {
	case Status::Normal: GetBorder().Restore(); break;
	case Status::Maximized: GetBorder().Hide(); break;
	}
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::ResizeLayer() {
	layer.Resize(region.size);
	Redraw(region_infinite);
}

void DesktopFrame::RecreateLayer() {
	layer.Create(hwnd, region.size);
	Redraw(region_infinite);
}

void DesktopFrame::Redraw(Rect redraw_region) {
	redraw_region = redraw_region.Intersect(Rect(point_zero, region.size));
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

void DesktopFrame::BorderFrame::OnMouseMsg(MouseMsg msg) {
	if (msg.type == MouseMsg::Move || msg.type == MouseMsg::LeftDown) {
		BorderPosition border_position = HitTestBorderPosition(size, border._width + border._radius, msg.point);
		if (msg.type == MouseMsg::Move) {
			SetCursor(GetBorderPositionCursor(border_position));
		} else {
			AeroSnapBorderResizingEffect(*this, border_position);
		}
	}
}


END_NAMESPACE(WndDesign)