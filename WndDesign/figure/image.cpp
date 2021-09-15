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

ComPtr<IWICFormatConverter> LoadImageFromFile(const std::wstring& file_name) {
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


Image::Image(const std::wstring& file_name) :
	bitmap(CreateD2DBitmapFromWicBitmap(LoadImageFromFile(file_name)).Detach()),
	size(SIZE2Size(static_cast<ID2D1Bitmap1*>(bitmap)->GetSize())) {
}

Image::Image(void* address, size_t size) :
	bitmap(CreateD2DBitmapFromWicBitmap(LoadImageFromMemory(address, size)).Detach()),
	size(SIZE2Size(static_cast<ID2D1Bitmap1*>(bitmap)->GetSize())) {
}

Image::~Image() { 
	SafeRelease(reinterpret_cast<ID2D1Bitmap1**>(&bitmap)); 
}


void ImageFigure::DrawOn(RenderTarget& target, Point point) const {
	target.DrawBitmap(
		static_cast<ID2D1Bitmap1*>(image.bitmap),
		Rect2RECT(Rect(point, image.size)),
		Opacity2Float(opacity),
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Rect2RECT(Rect(point_zero, image.size))
	);
}


void ImageRepeatFigure::DrawOn(RenderTarget& target, Point point) const {
	Size image_size = image.GetSize();
	Size target_size = GetTargetSize(target);
	Rect region_on_target = Rect(point_zero, target_size).Intersect(Rect(point_zero + offset, region.size));
	Vector target_to_image_space_conversion = (region.point - point_zero + offset_on_image) - offset;
	Rect region_on_image_space = region_on_target + target_to_image_space_conversion;
	Rect repeat_range = RegionToOverlappingTileRange(region_on_image_space, image_size);
	if (repeat_range.IsEmpty()) { return; }
	ImageFigure image_figure(image, opacity);
	for (RectPointIterator it(repeat_range); !it.Finished(); ++it) {
		Vector tile_offset = ScalePointBySize(it.Item(), image_size) - point_zero;
		Rect drawing_region_on_image_space = Rect(point_zero + tile_offset, image_size).Intersect(region_on_image_space);
		image_figure.region_on_image = drawing_region_on_image_space - tile_offset;
		image_figure.DrawOn(target, drawing_region_on_image_space.point - point_zero - target_to_image_space_conversion);
	}
}


END_NAMESPACE(WndDesign)