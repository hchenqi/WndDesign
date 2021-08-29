#include "LayerFrame.h"
#include "../figure/layer.h"


BEGIN_NAMESPACE(WndDesign)


LayerFrame::LayerFrame(child_ptr child) : WndFrame(std::move(child)) {}

LayerFrame::~LayerFrame() {}

const Size LayerFrame::OnSizeRefUpdate(Size size_ref) {
	Size child_size = UpdateChildSizeRef(child, size_ref);
	if (this->size != child_size) {
		this->size = child_size;
		layer.reset();
		cached_region.Clear();
	}
	return size;
}

void LayerFrame::OnChildSizeUpdate(const WndObject& child, Size child_size) {
	if (this->size != child_size) {
		this->size = child_size;
		layer.reset();
		cached_region.Clear();
		SizeUpdated(child_size);
	}
}

void LayerFrame::OnChildRedraw(const WndObject& child, Rect redraw_region) {
	cached_region.Sub(redraw_region);
	Redraw(redraw_region);
}

void LayerFrame::OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
	draw_region = Rect(point_zero, size).Intersect(draw_region); if (draw_region.IsEmpty()) { return; }
	Region uncached_region(draw_region); uncached_region.Sub(cached_region);
	if (!uncached_region.IsEmpty()) {
		if (layer == nullptr) { layer = std::make_unique<Layer>(size); }
		Rect redraw_region = uncached_region.GetBoundingRect();
		FigureQueue figure_queue; figure_queue.Begin();
		DrawChild(child, point_zero, figure_queue, redraw_region);
		figure_queue.End(); layer->DrawFigureQueue(figure_queue, vector_zero, redraw_region);
		cached_region.Union(redraw_region);
	}
	figure_queue.add(draw_region.point, new LayerFigure(*layer, draw_region));
}


END_NAMESPACE(WndDesign)