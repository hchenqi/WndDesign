#pragma once

#include "WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class TextBox : public WndObject {



private:
	virtual const Size UpdateLayout(Size size) override {
		return text_layout.AutoResize(size);
	}

	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region) override {
		figure_queue.append(point_zero, new TextLayoutFigure());
	}
};


END_NAMESPACE(WndDesign)