#include "desktop_layer.h"

#include "../system/d3d_api.h"
#include "../system/dxgi_api.h"
#include "../system/dcomp_api.h"
#include "../system/d2d_api.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


void DesktopLayer::Create(HANDLE hwnd, Size size) {
	Destroy();

	// Create swapchain.
	ComPtr<IDXGISwapChain1> swap_chain;
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = { 0 };
	swap_chain_desc.Width = size.width;
	swap_chain_desc.Height = size.height;
	swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_desc.Stereo = false;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swap_chain_desc.Flags = 0;
	swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	hr << GetDXGIFactory().CreateSwapChainForComposition(&GetD3DDevice(), &swap_chain_desc, nullptr, &swap_chain);
	this->swap_chain = static_cast<SwapChain*>(swap_chain.Detach());

	// Create bitmap.
	CreateBitmap();

	// Create DComp resource.
	ComPtr<IDCompositionVisual> comp_visual;
	hr << GetDCompDevice().CreateVisual(&comp_visual);
	comp_visual->SetContent(this->swap_chain);

	ComPtr<IDCompositionTarget> comp_target;
	hr << GetDCompDevice().CreateTargetForHwnd((HWND)hwnd, false, &comp_target);
	comp_target->SetRoot(comp_visual.Get());
	this->comp_target = static_cast<CompositionTarget*>(comp_target.Detach());

	GetDCompDevice().Commit();
}


void DesktopLayer::Destroy() {
	SafeRelease(&comp_target);
	DestroyBitmap();
	SafeRelease(&swap_chain);
}

void DesktopLayer::CreateBitmap() {
	ComPtr<IDXGISurface> dxgi_surface;
	hr << swap_chain->GetBuffer(0, IID_PPV_ARGS(&dxgi_surface));

	ComPtr<ID2D1Bitmap1> bitmap;
	D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	hr << GetD2DDeviceContext().CreateBitmapFromDxgiSurface(dxgi_surface.Get(), &bitmap_properties, &bitmap);
	this->bitmap.Set(static_cast<BitmapResource*>(bitmap.Detach()));
}

void DesktopLayer::DestroyBitmap() {
	bitmap.Destroy();
}

void DesktopLayer::Resize(Size size) {
	DestroyBitmap();
	hr << swap_chain->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0);
	CreateBitmap();
}

void DesktopLayer::Present() {
	DXGI_PRESENT_PARAMETERS present_parameters = {};
	hr << swap_chain->Present1(0, 0, &present_parameters);
}


END_NAMESPACE(WndDesign)