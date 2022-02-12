#include "LayerFrame.h"
#include "../geometry/helper.h"


BEGIN_NAMESPACE(WndDesign)


void _LayerFrame_Base::UpdateLayout() {
	layer.Destroy();
	invalid_region.Set(region_infinite);
}

Size _LayerFrame_Base::OnSizeRefUpdate(Size size_ref) {
	Size child_size = UpdateChildSizeRef(child, size_ref);
	if (size != child_size) { size = child_size; UpdateLayout(); }
	return size;
}

void _LayerFrame_Base::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (size != child_size) { size = child_size; UpdateLayout(); SizeUpdated(size); }
}

void _LayerFrame_Base::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	child_redraw_region = child_redraw_region.Intersect(Rect(point_zero, size));
	invalid_region.Union(child_redraw_region * scale);
	Redraw(child_redraw_region);
}

void _LayerFrame_Base::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	if (Scale current_scale = figure_queue.GetScale(); scale != current_scale) { scale = current_scale; layer.Destroy(); }
	Rect layer_draw_region = draw_region * scale, redraw_region = layer_draw_region;
	if (layer.IsEmpty()) {
		layer.Create(size * scale);
		invalid_region.Set(region_infinite);
	} else {
		Region render_region(redraw_region); render_region.Intersect(invalid_region);
		redraw_region = render_region.IsEmpty() ? region_empty : ceil(render_region.GetBoundingRect());
	}
	if (!redraw_region.IsEmpty()) {
		FigureQueue figure_queue([&](FigureQueue& figure_queue) {
			figure_queue.Group(scale, region_infinite, [&]() {
				DrawChild(child, point_zero, figure_queue, redraw_region * scale.Invert());
			});
		});
		layer.DrawFigureQueue(figure_queue, vector_zero, redraw_region);
		invalid_region.Sub(redraw_region);
	}
	figure_queue.add(draw_region.point, new LayerFigure(layer, layer_draw_region, draw_region.size));
}


END_NAMESPACE(WndDesign)