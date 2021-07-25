#include "d2d_hwnd.h"
#include "d3d_api.h"
#include "dxgi_api.h"
#include "dcomp_api.h"
#include "d2d_api.h"
#include "directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


HWNDTarget::HWNDTarget(HANDLE hwnd, Size size) : Target(nullptr), swap_chain(nullptr), comp_target(nullptr), comp_visual(nullptr) {
	// Create swapchain.
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

	// Create target.
	CreateTarget();

	// Create DComp resource.
	hr << GetDCompDevice().CreateTargetForHwnd((HWND)hwnd, false, &comp_target);
	hr << GetDCompDevice().CreateVisual(&comp_visual);
	comp_visual->SetContent(swap_chain);
	comp_target->SetRoot(comp_visual);
	GetDCompDevice().Commit();
}

HWNDTarget::~HWNDTarget() {
	SafeRelease(&comp_visual);
	SafeRelease(&comp_target);

	DestroyTarget();
	SafeRelease(&swap_chain);
}

void HWNDTarget::CreateTarget() {
	D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	ComPtr<IDXGISurface> dxgi_surface;
	hr << swap_chain->GetBuffer(0, IID_PPV_ARGS(&dxgi_surface));
	hr << GetD2DDeviceContext().CreateBitmapFromDxgiSurface(dxgi_surface.Get(), &bitmap_properties, &bitmap);
}

void HWNDTarget::DestroyTarget() {
	SafeRelease(&bitmap);
}

void HWNDTarget::OnResize(Size size) {
	DestroyTarget();
	hr << swap_chain->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0);
	CreateTarget();
}

void HWNDTarget::Present() {
	DXGI_PRESENT_PARAMETERS present_parameters = {};
	hr << swap_chain->Present1(0, 0, &present_parameters);
}


END_NAMESPACE(WndDesign)