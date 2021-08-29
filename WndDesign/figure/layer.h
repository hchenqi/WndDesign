#pragma once

#include "figure.h"
#include "../common/uncopyable.h"


BEGIN_NAMESPACE(WndDesign)

class FigureQueue;


class Layer : Uncopyable {
private:
	friend struct LayerFigure;
	Size size;
	alloc_ptr<void> bitmap;
public:
	Layer() : size(), bitmap(nullptr) {}
	~Layer();
	const Size GetSize() const { return size; }
	void SetSize(Size size);
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