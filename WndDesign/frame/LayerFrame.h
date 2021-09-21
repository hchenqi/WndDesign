#pragma once

#include "WndFrame.h"
#include "../geometry/region.h"
#include "../figure/layer.h"


BEGIN_NAMESPACE(WndDesign)


class LayerFrame : public WndFrame {
public:
	LayerFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	Size size;
	mutable Region invalid_region;
	mutable Layer layer;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override;
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override;
};


END_NAMESPACE(WndDesign)