#pragma once

#include "figure.h"
#include "../common/uncopyable.h"


struct ID2D1Bitmap1;


BEGIN_NAMESPACE(WndDesign)

class FigureQueue;


class Layer : Uncopyable {
protected:
	friend struct LayerFigure;
	alloc_ptr<ID2D1Bitmap1> bitmap;
	Layer() : bitmap(nullptr) {}
public:
	Layer(Size size);
	~Layer();
public:
	void DrawFigureQueue(const FigureQueue& figure_queue, Vector offset, Rect clip_region);
};


struct LayerFigure : Figure {
	const Layer& layer;
	Rect region;

	LayerFigure(const Layer& layer, Rect region) : layer(layer), region(region) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)