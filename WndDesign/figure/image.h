#pragma once

#include "figure.h"
#include "color.h"
#include "bitmap.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

struct ImageSource;


class Image : Uncopyable {
private:
	friend struct ImageFigure;
	Size size;
	alloc_ptr<ImageSource> source;
	Bitmap bitmap;
public:
	Image(const std::wstring& file_name);
	Image(void* address, size_t size);
	~Image();
	const Size GetSize() const { return size; }
	void CreateBitmap() const;
};


struct ImageFigure : Figure {
	const Image& image;
	uchar opacity;

	ImageFigure(const Image& image, uchar opacity = 0xFF) : image(image), opacity(opacity) { image.CreateBitmap(); }

	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)