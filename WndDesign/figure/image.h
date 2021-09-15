#pragma once

#include "figure.h"
#include "color.h"
#include "../common/uncopyable.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)


class Image : Uncopyable {
private:
	friend struct ImageFigure;
	Size size;
	alloc_ptr<void> bitmap;
public:
	Image(const std::wstring& file_name);
	Image(void* address, size_t size);
	~Image();
	const Size GetSize() const { return size; }
};


struct ImageFigure : Figure {
	const Image& image;
	uchar opacity;

	ImageFigure(const Image& image, uchar opacity = 0xFF) : image(image), opacity(opacity) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


struct ImageRepeatFigure : Figure {
	const Image& image;
	Rect region;
	uchar opacity;

	ImageRepeatFigure(const Image& image, Rect region, uchar opacity = 0xFF) : image(image), region(region), opacity(opacity) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)