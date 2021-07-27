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

ComPtr<IWICFormatConverter> LoadImageFromFile(const wstring& file_name) {
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

ComPtr<ID2D1Bitmap1> CreateD2DBitmapFromWicBitmap(ComPtr<IWICFormatConverter> converter) {
	ComPtr<ID2D1Bitmap1> bitmap;
	D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	hr << GetD2DDeviceContext().CreateBitmapFromWicBitmap(converter.Get(), &bitmap_properties, &bitmap);
	return bitmap;
}


END_NAMESPACE(Anonymous)


Image::Image(const wstring& file_name) :
	bitmap(CreateD2DBitmapFromWicBitmap(LoadImageFromFile(file_name)).Detach()),
	size(SIZE2Size(static_cast<ID2D1Bitmap1*>(bitmap)->GetSize())) {
}

Image::Image(void* address, size_t size) :
	bitmap(CreateD2DBitmapFromWicBitmap(LoadImageFromMemory(address, size)).Detach()),
	size(SIZE2Size(static_cast<ID2D1Bitmap1*>(bitmap)->GetSize())) {
}

Image::~Image() { 
	static_cast<ID2D1Bitmap1*>(bitmap)->Release(); bitmap = nullptr;
}


void ImageFigure::DrawOn(RenderTarget& target, Point point) const {
	if (opacity > 0) {
		target.DrawBitmap(
			static_cast<ID2D1Bitmap1*>(image.bitmap),
			Rect2RECT(Rect(point, image.GetSize())),
			Opacity2Float(opacity),
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			Rect2RECT(Rect(point_zero, image.GetSize()))
		);
	}
}


END_NAMESPACE(WndDesign)