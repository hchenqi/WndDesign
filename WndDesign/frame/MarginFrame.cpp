#include "MarginFrame.h"
#include "../style/style_helper.h"


BEGIN_NAMESPACE(WndDesign)


MarginFrame::MarginFrame(Style style, child_ptr child) :
	WndFrame(std::move(child)), style(style),
	margin(StyleHelper::CalculateBorderMargin(style.border) + StyleHelper::CalculatePaddingMargin(style.padding)) {
}

const Size MarginFrame::OnSizeRefUpdate(Size size_ref) {
	size = ExtendSizeByMargin(UpdateChildSizeRef(child, ShrinkSizeByMargin(size_ref, margin)), margin); return size;
}

void MarginFrame::OnChildSizeUpdate(WndObject& child, Size child_size) {
	size = ExtendSizeByMargin(child_size, margin); SizeUpdated(size);
}

ref_ptr<WndObject> MarginFrame::HitTest(Point& point) {
	if (ShrinkRegionByMargin(Rect(point_zero, size), margin).Contains(point)) { point -= GetClientOffset(); return child; }
	return nullptr;
}

void MarginFrame::OnChildRedraw(WndObject& child, Rect redraw_region) {
	Redraw(redraw_region + GetClientOffset());
}

void MarginFrame::OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
	DrawChild(child, point_zero + GetClientOffset(), figure_queue, draw_region);
	figure_queue.add(point_zero, StyleHelper::GetBorderFigure(size, style.border));
}


END_NAMESPACE(WndDesign)