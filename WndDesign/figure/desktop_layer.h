#pragma once

#include "layer.h"


BEGIN_NAMESPACE(WndDesign)

struct SwapChain;
struct CompositionTarget;


class DesktopLayer : private Layer {
public:
	using HANDLE = void*;
	alloc_ptr<SwapChain> swap_chain;
	alloc_ptr<CompositionTarget> comp_target;
public:
	DesktopLayer() : swap_chain(nullptr), comp_target(nullptr) {}
	~DesktopLayer() { Destroy(); }
private:
	void CreateBitmap();
	void DestroyBitmap();
public:
	void Create(HANDLE hwnd, Size size);
	void Destroy();
	void Resize(Size size);
	void Present(Rect dirty_rect);
	Layer::DrawFigureQueue;
};


END_NAMESPACE(WndDesign)