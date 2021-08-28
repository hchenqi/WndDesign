#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class TextBox;


template<>
class TextBox<Vertical, Relative> : public WndType<Relative, Auto> {

private:	
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		return layout.AutoResize(Size(size_ref.width, length_max));
	}

private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
		figure_queue.add(point_zero, new TextLayoutFigure(layout));
	}
};


template<>
class TextBox<Vertical, Auto> : public WndType<Auto, Auto> {

private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		return layout.AutoResize(Size(length_max, length_max));
	}
};


END_NAMESPACE(WndDesign)