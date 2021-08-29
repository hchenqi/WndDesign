#include "d3d_api.h"
#include "dxgi_api.h"
#include "dcomp_api.h"
#include "d2d_api.h"
#include "dwrite_api.h"
#include "wic_api.h"

#include "directx_helper.h"
#include "directx_resource.h"


BEGIN_NAMESPACE(WndDesign)


struct DirectXResource {
public:
	// D3D
	ComPtr<ID3D11Device> d3d_device;

	// DXGI
	ComPtr<IDXGIDevice1> dxgi_device;
	ComPtr<IDXGIAdapter> dxgi_adapter;
	ComPtr<IDXGIFactory2> dxgi_factory;

	// DComp
	ComPtr<IDCompositionDevice> dcomp_device;

	// D2D
	ComPtr<ID2D1Factory1> d2d_factory;
	ComPtr<ID2D1DeviceContext> d2d_device_context;
	ComPtr<ID2D1SolidColorBrush> d2d_solid_color_brush;

	// DWrite
	ComPtr<IDWriteFactory> dwrite_factory;

	// WIC
	ComPtr<IWICImagingFactory2> wic_factory;


private:
	static DirectXResource directx_resource;

private:
	DirectXResource();
	~DirectXResource();

	void CreateDeviceIndependentResource();
	void DiscardDeviceIndependentResource();
	void CreateDeviceDependentResource();
	void DiscardDeviceDependentResource();

public:
	static DirectXResource& Get() { return directx_resource; }

	void RecreateDeviceDependentResource() {
		DiscardDeviceDependentResource();
		CreateDeviceDependentResource();
	}
};


DirectXResource::DirectXResource() {
	hr << CoInitialize(nullptr);
	CreateDeviceIndependentResource();
	CreateDeviceDependentResource();
}

DirectXResource::~DirectXResource() {
	DiscardDeviceDependentResource();
	DiscardDeviceIndependentResource();
	CoUninitialize();
}

void DirectXResource::CreateDeviceIndependentResource() {
	// Create D2D factory.
	hr << D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &d2d_factory);

	// Create DWrite factory.
	hr << DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &dwrite_factory);

	// Create WIC factory.
	hr << CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory));
}

void DirectXResource::DiscardDeviceIndependentResource() {
	d2d_factory.Reset();
	dwrite_factory.Reset();
	wic_factory.Reset();
}

void DirectXResource::CreateDeviceDependentResource() {
	// Create D3D device.
	D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
	};
	D3D_FEATURE_LEVEL featureLevel;

	hr << D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT
	#ifdef _DEBUG
		| D3D11_CREATE_DEVICE_DEBUG
	#endif
		,
		featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&d3d_device,
		&featureLevel,
		nullptr
	);
	hr << d3d_device->QueryInterface(IID_PPV_ARGS(&dxgi_device));
	hr << dxgi_device->GetAdapter(&dxgi_adapter);
	hr << dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));

	// Create DComp device.
	hr << DCompositionCreateDevice(dxgi_device.Get(), IID_PPV_ARGS(&dcomp_device));

	// Create D2D device context.
	ComPtr<ID2D1Device> d2d_device;
	hr << d2d_factory->CreateDevice(dxgi_device.Get(), &d2d_device);
	hr << d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2d_device_context);

	// Create D2D brush.
	hr << d2d_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &d2d_solid_color_brush);
}

void DirectXResource::DiscardDeviceDependentResource() {
	d2d_solid_color_brush.Reset();
	d2d_device_context.Reset();

	dcomp_device.Reset();

	dxgi_factory.Reset();
	dxgi_adapter.Reset();
	dxgi_device.Reset();
#ifdef _DEBUG
	ComPtr<ID3D11Debug> d3d_debug;
	hr << d3d_device.As(&d3d_debug);
	d3d_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	d3d_debug.Reset();
#endif
	d3d_device.Reset();
}


void DirectXRecreateResource() { DirectXResource::Get().RecreateDeviceDependentResource(); }

ID3D11Device& GetD3DDevice() { return *DirectXResource::Get().d3d_device.Get(); }

IDXGIDevice1& GetDXGIDevice() { return *DirectXResource::Get().dxgi_device.Get(); }
IDXGIAdapter& GetDXGIAdapter() { return *DirectXResource::Get().dxgi_adapter.Get(); }
IDXGIFactory2& GetDXGIFactory() { return *DirectXResource::Get().dxgi_factory.Get(); }

IDCompositionDevice& GetDCompDevice() { return *DirectXResource::Get().dcomp_device.Get(); }

ID2D1Factory1& GetD2DFactory() { return *DirectXResource::Get().d2d_factory.Get(); }
ID2D1DeviceContext& GetD2DDeviceContext() { return *DirectXResource::Get().d2d_device_context.Get(); }
ID2D1SolidColorBrush& GetD2DSolidColorBrush() { return *DirectXResource::Get().d2d_solid_color_brush.Get(); }

IDWriteFactory& GetDWriteFactory() { return *DirectXResource::Get().dwrite_factory.Get(); }

IWICImagingFactory2& GetWICFactory() { return *DirectXResource::Get().wic_factory.Get(); }


ID2D1SolidColorBrush& GetD2DSolidColorBrush(Color color) {
	ID2D1SolidColorBrush& brush = GetD2DSolidColorBrush();
	brush.SetColor(Color2COLOR(color));
	return brush;
}

void BeginDraw() {
	GetD2DDeviceContext().BeginDraw();
}

void EndDraw() {
	hr << GetD2DDeviceContext().EndDraw();
	GetD2DDeviceContext().SetTarget(nullptr);
}


END_NAMESPACE(WndDesign)