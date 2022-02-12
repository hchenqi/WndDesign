#pragma once

#include "wrapper.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd, Color color = Color::White>
class SolidColorBackground : public Wnd {
public:
	using Wnd::Wnd;
	using Base = SolidColorBackground;

	// style
protected:
	Color background = color;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, background));
		Wnd::OnDraw(figure_queue, draw_region);
	}
};


END_NAMESPACE(WndDesign)