#pragma once

#include "WndFrame.h"
#include "../figure/layer.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class LayerFrame : public WndFrame<WidthType, HeightType> {
public:
	LayerFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Layer layer;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { 
		Size size = UpdateChildSizeRef(child, size_ref); 
		layer.SetSize(size);
		return size;
	}
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {
		layer.SetSize(child_size);
		SizeUpdated(child_size);
	}
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override {
		redraw_region = Rect(point_zero, layer.GetSize()).Intersect(redraw_region); if (redraw_region.IsEmpty()) { return; }
		FigureQueue figure_queue; figure_queue.Begin();
		DrawChild(child, point_zero, figure_queue, redraw_region);
		figure_queue.End(); layer.DrawFigureQueue(figure_queue, vector_zero, redraw_region);
		Redraw(redraw_region);
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {
		figure_queue.add(draw_region.point, new LayerFigure(layer, draw_region));
	}
};


END_NAMESPACE(WndDesign)