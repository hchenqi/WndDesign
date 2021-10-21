#include "DesktopFrame.h"
#include "../window/desktop.h"
#include "../style/style_helper.h"
#include "../style/border_helper.h"
#include "../figure/desktop_layer.h"
#include "../figure/shape.h"
#include "../system/win32_api.h"
#include "../system/win32_aero_snap.h"
#include "../system/d2d_api.h"
#include "../system/directx_resource.h"


BEGIN_NAMESPACE(WndDesign)


DesktopFrame::DesktopFrame(Style style, child_ptr child) :
	BorderFrame(style.border._width, style.border._radius, style.border._color, std::move(child)), width(style.width), height(style.height) {
	Rect region = StyleHelper::CalculateRegion(style.width, style.height, style.position, Win32::GetDesktopSize());
	point = region.point; BorderFrame::OnSizeRefUpdate(region.size);
	hwnd = Win32::CreateWnd(region, style.title); Win32::SetWndUserData(hwnd, this);
	RecreateLayer();
}

DesktopFrame::~DesktopFrame() {
	layer.Destroy();
	Win32::SetWndUserData(hwnd, nullptr);
	Win32::DestroyWnd(hwnd);
}

std::pair<Size, Rect> DesktopFrame::GetMinMaxRegion() const {
	Size desktop_size = Win32::GetDesktopSize();
	auto [size_min, size_max] = StyleHelper::CalculateMinMaxSize(width, height, desktop_size);
	Rect region_max(point_zero, size_max);
	if (size_max == desktop_size) { region_max = ExtendRegion(region_max, border_width); }
	return { size_min, region_max };
}

void DesktopFrame::SetTitle(std::wstring title) { Win32::SetWndTitle(hwnd, title); }

void DesktopFrame::SetSize(Size size) {
	if (this->size != size) {
		BorderFrame::OnSizeRefUpdate(size);
		ResizeLayer();
	}
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::ResizeLayer() {
	layer.Resize(size);
	invalid_region.Set(Rect(point_zero, size));
	Redraw(region_infinite);
}

void DesktopFrame::RecreateLayer() {
	layer.Create(hwnd, size);
	invalid_region.Set(Rect(point_zero, size));
	Redraw(region_infinite);
}

void DesktopFrame::Redraw(Rect redraw_region) {
	redraw_region = redraw_region.Intersect(Rect(point_zero, size));
	invalid_region.Union(redraw_region);
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::OnChildRedraw(WndObject& child, Rect redraw_region) {
	Redraw(redraw_region + GetChildOffset());
}

void DesktopFrame::Draw() {
	Rect render_rect = invalid_region.GetBoundingRect(); if (render_rect.IsEmpty()) { return; }
	BeginDraw();
	FigureQueue figure_queue; figure_queue.Begin();
	BorderFrame::OnDraw(figure_queue, render_rect);
	figure_queue.End(); layer.DrawFigureQueue(figure_queue, vector_zero, render_rect);
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
	if (msg.type == MouseMsg::Move || msg.type == MouseMsg::LeftDown) {
		BorderPosition border_position = HitTestBorderPosition(size, border_width + border_radius, msg.point);
		if (msg.type == MouseMsg::Move) {
			SetCursor(GetBorderPositionCursor(border_position));
		} else {
			AeroSnapBorderResizingEffect(*this, msg.point, border_position);
		}
	}
}

END_NAMESPACE(WndDesign)