#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"


struct ID2D1Bitmap1;


BEGIN_NAMESPACE(WndDesign)

class FigureQueue;


class Target : Uncopyable {
protected:
	alloc_ptr<ID2D1Bitmap1> bitmap;
	Target(nullptr_t) : bitmap(nullptr) {}
public:
	Target(Size size);
	~Target();
	ID2D1Bitmap1& GetBitmap() const { return *bitmap; }
	void DrawFigureQueue(const FigureQueue& figure_queue, Vector offset, Rect clip_region);
public:
	static void BeginDraw();
	static void EndDraw() {}
};


END_NAMESPACE(WndDesign)