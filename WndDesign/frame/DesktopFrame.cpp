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


DesktopFrame::DesktopFrame(DesktopFrameStyle style, child_ptr child) : style(style), child(std::move(child)), hwnd(nullptr) {
	RegisterChild(this->child);
	region = StyleHelper::CalculateRegion(style.width, style.height, style.position, desktop.GetSize()); 
	UpdateClientRegion(); UpdateChildSizeRef(this->child, client_region.size);
	hwnd = Win32::CreateWnd(region, style.title); Win32::SetWndUserData(hwnd, this);
	RecreateLayer();
}

DesktopFrame::~DesktopFrame() {
	layer.Destroy();
	Win32::SetWndUserData(hwnd, nullptr);
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
		UpdateClientRegion(); UpdateChildSizeRef(child, client_region.size);
		layer.Resize(new_region.size);
		invalid_region.Set(Rect(point_zero, region.size));
		Redraw(region_infinite);
	}
	region.point = new_region.point;
}

void DesktopFrame::UpdateClientRegion() {
	client_region = ShrinkRegionByMargin(Rect(point_zero, region.size), StyleHelper::CalculateBorderMargin(style.border));
}

ref_ptr<WndObject> DesktopFrame::HitTest(Point& point) {
	if (PointInRoundedRectangle(point, client_region, style.border._radius)) {
		point -= client_region.point - point_zero;
		return child;
	}
	return this;
}

void DesktopFrame::Show() { Win32::ShowWnd(hwnd); }
void DesktopFrame::Minimize() { Win32::MinimizeWnd(hwnd); }
void DesktopFrame::Maximize() { Win32::MaximizeWnd(hwnd); }
void DesktopFrame::Restore() { Win32::RestoreWnd(hwnd); }
void DesktopFrame::Destroy() { desktop.RemoveChild(*this); }

void DesktopFrame::Redraw(Rect redraw_region) {
	redraw_region = redraw_region.Intersect(Rect(point_zero, region.size));
	invalid_region.Union(redraw_region);
	Win32::InvalidateWndRegion(hwnd, redraw_region);
}

void DesktopFrame::OnChildRedraw(WndObject& child, Rect redraw_region) { 
	Redraw(redraw_region + GetClientOffset()); 
}

void DesktopFrame::Draw() {
	Rect render_rect = invalid_region.GetBoundingRect(); if (render_rect.IsEmpty()) { return; }
	BeginDraw();
	FigureQueue figure_queue; figure_queue.Begin();
	DrawChild(child, client_region.point, figure_queue, render_rect.Intersect(client_region));
	figure_queue.add(point_zero, StyleHelper::GetBorderFigure(region.size, style.border));
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
		BorderPosition border_position = HitTestBorderPosition(region.size, style.border._width + style.border._radius, msg.point);
		if (msg.type == MouseMsg::Move) {
			SetCursor(GetBorderPositionCursor(border_position));
		} else {
			AeroSnapBorderResizingEffect(*this, msg.point, border_position);
		}
	}
}

void DesktopFrame::RecreateLayer() {
	layer.Create(hwnd, region.size);
	invalid_region.Set(Rect(point_zero, region.size));
	Redraw(region_infinite);
}


END_NAMESPACE(WndDesign)