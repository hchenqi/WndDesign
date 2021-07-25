#include "d2d_target.h"
#include "d2d_api.h"
#include "directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


inline alloc_ptr<ID2D1Bitmap1> D2DCreateBitmap(Size size) {
    D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    ComPtr<ID2D1Bitmap1> bitmap;
    hr << GetD2DDeviceContext().CreateBitmap(D2D1::SizeU(size.width, size.height), nullptr, 0, &bitmap_properties, &bitmap);
    return bitmap.Detach();
}


Target::Target(Size size) : bitmap(D2DCreateBitmap(size)) {}

Target::~Target() { SafeRelease(&bitmap); }


void Target::BeginDraw() {
    auto& device_context = GetD2DDeviceContext();
    device_context.BeginDraw();
}

void Target::EndDraw() {
    auto& device_context = GetD2DDeviceContext();
    hr << device_context.EndDraw();
    device_context.SetTarget(nullptr);
}


END_NAMESPACE(WndDesign)