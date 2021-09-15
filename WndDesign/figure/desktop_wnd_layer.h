#pragma once

#include "layer.h"


struct IDXGISwapChain1;

struct IDCompositionTarget;
struct IDCompositionVisual;


BEGIN_NAMESPACE(WndDesign)


class DesktopLayer : private Layer {
public:
	using HANDLE = void*;
public:
	DesktopLayer(HANDLE hwnd, Size size);
	~DesktopLayer();
	void OnResize(Size size);
	void Present();
	Layer::DrawFigureQueue;
private:
	alloc_ptr<IDXGISwapChain1> swap_chain;
	alloc_ptr<IDCompositionTarget> comp_target;
	alloc_ptr<IDCompositionVisual> comp_visual;
private:
	void CreateTarget();
	void DestroyTarget();
};


END_NAMESPACE(WndDesign)