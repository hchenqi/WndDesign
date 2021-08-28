#pragma once

#include "figure.h"
#include "color.h"
#include "../common/uncopyable.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

using std::wstring;


class Image : Uncopyable {
private:
	friend struct ImageFigure;
	alloc_ptr<void> bitmap;
	Size size;
public:
	Image(const wstring& file_name);
	Image(void* address, size_t size);
	~Image();
	const Size GetSize() const { return size; }
};


struct ImageFigure : Figure {
	const Image& image;
	uchar opacity;

	ImageFigure(const Image& image, uchar opacity = 0xFF) :image(image), opacity(opacity) {}

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)