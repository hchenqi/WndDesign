#include "image.h"
#include "../geometry/tiling.h"
#include "../system/d2d_api.h"
#include "../system/wic_api.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)


ComPtr<IWICFormatConverter> LoadFromDecoder(ComPtr<IWICBitmapDecoder> decoder) {
	ComPtr<IWICFormatConverter> converter;
	ComPtr<IWICBitmapFrameDecode> source;
	hr << GetWICFactory().CreateFormatConverter(&converter);
	hr << decoder->GetFrame(0, &source);
	hr << converter->Initialize(
		source.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeMedianCut
	);
	return converter;
}

ComPtr<IWICFormatConverter> LoadImageFromFile(std::wstring file_name) {
	try {
		ComPtr<IWICBitmapDecoder> decoder;
		hr << GetWICFactory().CreateDecoderFromFilename(
			file_name.c_str(),
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decoder
		);
		return LoadFromDecoder(decoder);
	} catch (std::runtime_error&) {
		throw std::invalid_argument("invalid image file");
	}
}

ComPtr<IWICFormatConverter> LoadImageFromMemory(void* address, size_t size) {
	try {
		ComPtr<IWICStream> stream;
		ComPtr<IWICBitmapDecoder> decoder;
		hr << GetWICFactory().CreateStream(&stream);
		hr << stream->InitializeFromMemory(reinterpret_cast<BYTE*>(address), (DWORD)size);
		hr << GetWICFactory().CreateDecoderFromStream(
			stream.Get(),
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&decoder
		);
		return LoadFromDecoder(decoder);
	} catch (std::runtime_error&) {
		throw std::invalid_argument("invalid image address");
	}
}


inline Size GetImageSize(ImageSource& source) {
	uint width, height;
	hr << source.GetSize(&width, &height);
	return Size((float)width, (float)height);
}

inline ComPtr<ID2D1Bitmap1> CreateD2DBitmapFromWicBitmap(IWICFormatConverter& converter) {
	ComPtr<ID2D1Bitmap1> bitmap;
	D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	hr << GetD2DDeviceContext().CreateBitmapFromWicBitmap(&converter, &bitmap_properties, &bitmap);
	return bitmap;
}


END_NAMESPACE(Anonymous)


Image::Image(std::wstring file_name) :
	source(static_cast<ImageSource*>(LoadImageFromFile(file_name).Detach())),
	size(GetImageSize(*source)) {
}

Image::Image(void* address, size_t size) :
	source(static_cast<ImageSource*>(LoadImageFromMemory(address, size).Detach())),
	size(GetImageSize(*source)) {
}

Image::~Image() {
	SafeRelease(&source);
}

void Image::CreateBitmap() const {
	if (bitmap.IsEmpty()) {
		bitmap.Set(static_cast<BitmapResource*>(CreateD2DBitmapFromWicBitmap(*source).Detach()));
	}
}


void ImageFigure::DrawOn(RenderTarget& target, Point point) const {
	target.DrawBitmap(
		image.bitmap.Get(),
		AsD2DRect(Rect(point, region.size)),
		OpacityAsFloat(opacity),
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		AsD2DRect(region)
	);
}


void ImageRepeatFigure::DrawOn(RenderTarget& target, Point point) const {
	Size image_size = image.GetSize();
	Rect region_on_target = Rect(point_zero, GetTargetSize(target)).Intersect(Rect(point, region.size));
	Rect region_on_image = Rect(region.point, region_on_target.size);
	ImageFigure image_figure(image, region_empty, opacity);
	for (RectPointIterator it(RegionToOverlappingTileRange(region_on_image, image_size)); it; ++it) {
		Point tile_offset = ScalePointBySize(*it, image_size);
		Rect tile_region_on_image = Rect(tile_offset, image_size).Intersect(region_on_image);
		image_figure.region = tile_region_on_image - (tile_offset - point_zero);
		image_figure.DrawOn(target, tile_region_on_image.point + (region_on_target.point - region_on_image.point));
	}
}


END_NAMESPACE(WndDesign)