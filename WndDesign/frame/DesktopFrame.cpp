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
	Base(std::move(child)), width(style.width), height(style.height) {
	border = style.border;
	hwnd = Win32::CreateWnd(region_empty, style.title);
	scale_x = scale_y = Win32::GetWndDpiScale(hwnd);
	Size desktop_size = Scale(Win32::GetDesktopSize(), 1.0f / scale_x, 1.0f / scale_y);
	region = StyleHelper::CalculateRegion(style.width, style.height, style.position, desktop_size);
	region = Scale(region, scale_x, scale_y);
	Base::OnSizeRefUpdate(region.size); Base::GetSize();
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
	Size desktop_size = Scale(Win32::GetDesktopSize(), 1.0f / scale_x, 1.0f / scale_y);
	auto [size_min, size_max] = StyleHelper::CalculateMinMaxSize(width, height, desktop_size);
	Rect region_max(point_zero, size_max);
	if (size_max == desktop_size) { region_max = Extend(region_max, border._width); }
	return { Scale(size_min, scale_x, scale_y), Scale(region_max, scale_x, scale_y) };
}

void DesktopFrame::SetTitle(std::wstring title) { Win32::SetWndTitle(hwnd, title); }

void DesktopFrame::SetSize(Size size) {
	if (region.size != size) {
		region.size = size;
		Base::OnSizeRefUpdate(size); Base::GetSize();
		ResizeLayer();
	}
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::ResizeLayer() {
	layer.Resize(region.size);
	invalid_region.Set(Rect(point_zero, region.size));
	Redraw(region_infinite);
}

void DesktopFrame::RecreateLayer() {
	layer.Create(hwnd, region.size);
	invalid_region.Set(Rect(point_zero, region.size));
	Redraw(region_infinite);
}

void DesktopFrame::Redraw(Rect redraw_region) {
	redraw_region = redraw_region.Intersect(Rect(point_zero, region.size));
	invalid_region.Union(redraw_region);
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Redraw(Scale(child_redraw_region + GetInnerOffset(), scale_x, scale_y));
}

void DesktopFrame::Draw() {
	Rect render_rect = invalid_region.GetBoundingRect(); if (render_rect.IsEmpty()) { return; }
	BeginDraw();
	FigureQueue figure_queue([&](FigureQueue& figure_queue) { Base::OnDraw(figure_queue, render_rect); });
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

void DesktopFrame::OnMouseMsg(MouseMsg msg) {
	msg.point = Scale(msg.point, 1.0f / scale_x, 1.0f / scale_y);
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