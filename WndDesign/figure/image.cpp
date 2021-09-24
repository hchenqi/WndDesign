#include "image.h"

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
		throw std::invalid_argument("invalid iamge file");
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
		throw std::invalid_argument("invalid iamge address");
	}
}


inline Size GetImageSize(ImageSource& source) {
	Size size; hr << source.GetSize(&size.width, &size.height); return size;
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

void Image::CreateBitmap() {
	if (bitmap.IsEmpty()) {
		bitmap.Set(static_cast<BitmapResource*>(CreateD2DBitmapFromWicBitmap(*source).Detach()));
	}
}


void ImageFigure::DrawOn(RenderTarget& target, Point point) const {
	target.DrawBitmap(
		image.bitmap.Get(),
		Rect2RECT(Rect(point, image.size)),
		Opacity2Float(opacity),
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Rect2RECT(Rect(point_zero, image.size))
	);
}


END_NAMESPACE(WndDesign)