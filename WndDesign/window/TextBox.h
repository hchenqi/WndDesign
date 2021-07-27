#pragma once

#include "WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class TextBoxHorizontal : public WndObject {



private:
	virtual const Size UpdateLayout(Size size) override {
		return layout.AutoResize(Size(size.width, length_max));
	}

	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region) override {
		figure_queue.append(point_zero, new TextLayoutFigure(layout));
	}
};


END_NAMESPACE(WndDesign)