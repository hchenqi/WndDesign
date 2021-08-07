#pragma once

#include "../window/WndObject.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)

using std::vector;


class ListLayoutVertical : public WndObject {
private:
	struct ItemInfo {
		int y;
		uint height;


	};
	vector<ItemInfo> items;


private:
	virtual const Size UpdateLayout() {

	}


	virtual void InvalidateChildSize(WndObject& child) {


		InvalidateLayout();
	}



	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region) override {




	}

};


END_NAMESPACE(WndDesign)