#pragma once

#include "WndFrame.h"
#include "../figure/layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class _LayerFrame_Base : public WndFrame {
protected:
	using WndFrame::WndFrame;

	// layout
protected:
	Size size;
protected:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;

	// paint
protected:
	Scale scale;
	Layer layer;
	Region invalid_region;
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<class WidthType, class HeightType>
class LayerFrame : public _LayerFrame_Base, public LayoutType<WidthType, HeightType> {
public:
	LayerFrame(child_ptr<WidthType, HeightType> child) : _LayerFrame_Base(std::move(child)) {}
};


template<class T>
LayerFrame(T)->LayerFrame<extract_width_type<T>, extract_height_type<T>>;


END_NAMESPACE(WndDesign)