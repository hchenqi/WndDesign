#pragma once

#include "figure.h"
#include "bitmap.h"


BEGIN_NAMESPACE(WndDesign)

class FigureQueue;


class Layer : Uncopyable {
protected:
	friend struct LayerFigure;
	Bitmap bitmap;
public:
	Layer() {}
	~Layer() {}
	bool IsEmpty() const { return bitmap.IsEmpty(); }
	void Create(Size size);
	void Destroy() { bitmap.Destroy(); }
public:
	void DrawFigureQueue(const FigureQueue& figure_queue, Vector offset, Rect clip_region);
};


struct LayerFigure : Figure {
	const Layer& layer;
	Rect region;
	Size size;
	uchar opacity;

	LayerFigure(const Layer& layer, Rect region, uchar opacity = 0xFF) : LayerFigure(layer, region, region.size, opacity) {}
	LayerFigure(const Layer& layer, Rect region, Size size, uchar opacity = 0xFF) : layer(layer), region(region), size(size), opacity(opacity) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)