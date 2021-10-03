#pragma once

#include "../geometry/geometry.h"
#include "../figure/color.h"

#include <d2d1_1.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <dcomp.h>
#include <dwrite_3.h>
#include <wincodec.h>

#include <wrl/client.h>


BEGIN_NAMESPACE(WndDesign)


using Microsoft::WRL::ComPtr;


struct HResultWrapper {
	HResultWrapper& operator<<(HRESULT hr) {
		if (!SUCCEEDED(hr)) { throw std::runtime_error("hresult failure"); }
		return *this;
	}
};

inline static HResultWrapper hr;


template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != nullptr) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}


// resource aliases
struct RenderTarget : ID2D1DeviceContext {};
struct BitmapResource : ID2D1Bitmap1 {};
struct SwapChain : IDXGISwapChain1 {};
struct CompositionTarget : IDCompositionTarget {};
struct ImageSource : IWICFormatConverter {};
struct TextLayout : IDWriteTextLayout4 {};


inline D2D1_POINT_2F Point2POINT(Point point) {
	return D2D1::Point2F(static_cast<FLOAT>(point.x), static_cast<FLOAT>(point.y));
}

inline D2D1_SIZE_F Size2SIZE(Size size) {
	return D2D1::SizeF(static_cast<FLOAT>(size.width), static_cast<FLOAT>(size.height));
}

inline D2D1_RECT_F Rect2RECT(Rect rect) {
	return D2D1::RectF(
		static_cast<FLOAT>(rect.left()), static_cast<FLOAT>(rect.top()),
		static_cast<FLOAT>(rect.right()), static_cast<FLOAT>(rect.bottom())
	);
}

inline Point POINT2Point(D2D1_POINT_2F point) {
	return Point(static_cast<uint>(point.x), static_cast<uint>(point.y));
}

inline Size SIZE2Size(D2D1_SIZE_F size) {
	return Size(static_cast<uint>(size.width), static_cast<uint>(size.height));
}

inline Rect RECT2Rect(D2D1_RECT_F rect) {
	return Rect(
		static_cast<int>(rect.left),
		static_cast<int>(rect.top),
		static_cast<uint>(static_cast<int>(rect.right) - static_cast<int>(rect.left)),
		static_cast<uint>(static_cast<int>(rect.bottom) - static_cast<int>(rect.top))
	);
}

inline float Opacity2Float(uchar opacity) {
	return opacity / (float)0xFF;
}

inline D2D1_COLOR_F Color2COLOR(Color color) {
	return D2D1::ColorF(color.AsUnsigned(), Opacity2Float(color.alpha));
}


END_NAMESPACE(WndDesign)