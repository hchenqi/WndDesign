#pragma once

#include "../window/wnd_traits.h"
#include "../figure/image.h"


BEGIN_NAMESPACE(WndDesign)


class ImageBox : public WndType<Auto, Auto> {
public:
	ImageBox(std::wstring image_file) : image(image_file) {}
private:
	Image image;
protected:
	virtual Size GetSize() override { return image.GetSize(); }
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		draw_region = draw_region.Intersect(Rect(point_zero, GetSize())); if (draw_region.IsEmpty()) { return; }
		figure_queue.add(draw_region.point, new ImageFigure(image, draw_region));
	}
};


class ImageRepeatBox : public WndType<Assigned, Assigned> {
public:
	ImageRepeatBox(std::wstring image_file, Point offset = point_zero) : image(image_file), offset(offset - point_zero) {}
private:
	Image image;
	Vector offset;
private:
	Size size;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size = size_ref; }
	virtual Size GetSize() override { return size; }
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new ImageRepeatFigure(image, Rect(draw_region.point + offset, draw_region.size)));
	}
};


END_NAMESPACE(WndDesign)