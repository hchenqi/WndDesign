#include "LayerFrame.h"


BEGIN_NAMESPACE(WndDesign)


const Size LayerFrame::OnSizeRefUpdate(Size size_ref) {
	Size child_size = UpdateChildSizeRef(child, size_ref);
	if (size != child_size) {
		size = child_size;
		layer.Destroy();
		invalid_region.Set(Rect(point_zero, size));
	}
	return size;
}

void LayerFrame::OnChildSizeUpdate(const WndObject& child, Size child_size) {
	if (size != child_size) {
		size = child_size;
		layer.Destroy();
		invalid_region.Set(Rect(point_zero, size));
		SizeUpdated(child_size);
	}
}

void LayerFrame::OnChildRedraw(const WndObject& child, Rect redraw_region) {
	redraw_region = redraw_region.Intersect(Rect(point_zero, size));
	invalid_region.Union(redraw_region);
	Redraw(redraw_region);
}

void LayerFrame::OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	Region render_region(draw_region); render_region.Intersect(invalid_region);
	if (!render_region.IsEmpty()) {
		if (layer.IsEmpty()) { layer.Create(size); }
		Rect render_rect = render_region.GetBoundingRect();
		FigureQueue figure_queue; figure_queue.Begin();
		DrawChild(child, point_zero, figure_queue, render_rect);
		figure_queue.End(); layer.DrawFigureQueue(figure_queue, vector_zero, render_rect);
		invalid_region.Sub(render_rect);
	}
	figure_queue.add(draw_region.point, new LayerFigure(layer, draw_region));
}


END_NAMESPACE(WndDesign)