#pragma once

#include "WndFrame.h"
#include "../geometry/region.h"
#include "../figure/layer.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class LayerFrame : public WndFrame, public LayoutType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	LayerFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size;
	Region invalid_region;
	Layer layer;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		Size child_size = UpdateChildSizeRef(child, size_ref);
		if (size != child_size) {
			size = child_size;
			layer.Destroy();
			invalid_region.Set(Rect(point_zero, size));
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (size != child_size) {
			size = child_size;
			layer.Destroy();
			invalid_region.Set(Rect(point_zero, size));
			SizeUpdated(child_size);
		}
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		redraw_region = redraw_region.Intersect(Rect(point_zero, size));
		invalid_region.Union(redraw_region);
		Redraw(redraw_region);
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
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
};


END_NAMESPACE(WndDesign)