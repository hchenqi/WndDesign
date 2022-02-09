#include "OverlapLayout.h"
#include "../common/reversion_wrapper.h"


BEGIN_NAMESPACE(WndDesign)


void OverlapLayout::AddChild(frame_ptr frame) {
	RegisterChild(*frame);
	auto& frame_ref = *frame_list.emplace_back(std::move(frame));
	frame_ref.UpdateLayout(UpdateChildSizeRef(frame_ref, size), size);
}

void OverlapLayout::RemoveChild(frame_ref frame) {
	auto it = std::find_if(frame_list.begin(), frame_list.end(), [&](const frame_ptr& ptr) { return ptr.get() == &frame; });
	if (it == frame_list.end()) { throw std::invalid_argument("invalid overlap frame"); }
	Rect region = (*it)->region;
	frame_list.erase(it);
	Redraw(region);
}

Size OverlapLayout::OnSizeRefUpdate(Size size_ref) {
	size = size_ref;
	for (auto& frame : frame_list) { frame->UpdateLayout(UpdateChildSizeRef(*frame, size), size); }
	return size;
}

void OverlapLayout::OnChildSizeUpdate(WndObject& child, Size child_size) {
	Redraw(AsFrame(child).UpdateLayout(child_size, size));
}

ref_ptr<WndObject> OverlapLayout::HitTest(Point& point) {
	for (auto& frame : reverse(frame_list)) {
		if (frame->region.Contains(point)) {
			point -= frame->region.point - point_zero;
			return frame.get();
		}
	}
	return this;
}

Transform OverlapLayout::GetChildTransform(WndObject& child) const {
	return AsFrame(child).region.point - point_zero;
}

void OverlapLayout::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	for (auto& frame : frame_list) {
		DrawChild(*frame, frame->region, figure_queue, draw_region);
	}
}


END_NAMESPACE(WndDesign)